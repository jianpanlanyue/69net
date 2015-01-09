#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <cmath>
#include <cassert>
#include <vector>
#include "PriorityQueue.h"
#include "OpenCloseMap.h"
#include "Map.h"
#include "PathNode.h"

template<typename T>
struct AStar
{
    Map<T>* originalMap = nullptr;                          // 指向原地图数据
    int w = 0, h = 0;

    typedef PathNode<T*>    PNT;
    typedef PathNode<T*>*   PPNT;
    Map<PNT>*               pathNodeMap = nullptr;      // 通过原地图数据填充
    OpenCloseMap<PPNT>*     closedSet = nullptr;
    OpenCloseMap<PPNT>*     openSet = nullptr;
    PriorityQueue<PPNT>*    orderedOpenSet = nullptr;
    Map<PPNT>*              cameFrom = nullptr;
    OpenCloseMap<PPNT>*     runtimeGrid = nullptr;

    AStar( Map<T>* m )
    {
        assert( m );
        w = m->w;
        h = m->h;
        originalMap = m;
        pathNodeMap = new Map<PNT>( w, h );
        closedSet = new OpenCloseMap<PPNT>( w, h );
        openSet = new OpenCloseMap<PPNT>( w, h );
        orderedOpenSet = new PriorityQueue<PPNT>(); // todo: opacity ?
        cameFrom = new Map<PPNT>( w, h );
        runtimeGrid = new OpenCloseMap<PPNT>( w, h );

        for( int x = 0; x < w; x++ )
        {
            for( int y = 0; y < h; y++ )
            {
                pathNodeMap->At( x, y ).Assign( x, y, &m->At( x, y ) );
            }
        }
    }
    ~AStar()
    {
        delete pathNodeMap;
        delete closedSet;
        delete openSet;
        delete orderedOpenSet;
        delete cameFrom;
        delete runtimeGrid;

        originalMap = nullptr;
        pathNodeMap = nullptr;
        closedSet = nullptr;
        openSet = nullptr;
        orderedOpenSet = nullptr;
        cameFrom = nullptr;
        runtimeGrid = nullptr;
    }

    bool Search( int aX, int aY, int bX, int bY, std::vector<T*>& resultContainer )
    {
        auto startNode = &pathNodeMap->At( aX, aY );
        auto endNode = &pathNodeMap->At( bX, bY );

        if( startNode == endNode )
        {
            resultContainer.clear();
            return true;
        }

        closedSet->Clear();
        openSet->Clear();
        orderedOpenSet->Clear();
        cameFrom->All( []( PPNT& pn ) { pn = nullptr; } );
        runtimeGrid->Clear();

        startNode->g = 0;
        startNode->h = Heuristic( startNode, endNode );
        startNode->f = startNode->h;

        openSet->Add( startNode );
        orderedOpenSet->Push( startNode );
        runtimeGrid->Add( startNode );

        std::vector<PPNT> neighbors;
        while( openSet->c )
        {
            auto x = orderedOpenSet->Pop();
            if( x == endNode )
            {
                ReconstructPath( cameFrom->At( endNode->x, endNode->y ), resultContainer );
                return true;
            }

            openSet->Remove( x );
            closedSet->Add( x );

            FillNeighbors( x, neighbors );

            for( auto y : neighbors )
            {
                if( !y
                    || !x->userContext->IsWalkable( *y->userContext )
                    || closedSet->Contains( y ) ) continue;

                bool better, added = false;

                double score = runtimeGrid->At( x->x, x->y )->g + NeighborDistance( x, y );

                if( !openSet->Contains( y ) )
                {
                    openSet->Add( y );
                    better = true;
                    added = true;
                }
                else if( score < runtimeGrid->At( y->x, y->y )->g )
                {
                    better = true;
                }
                else
                {
                    better = false;
                }

                if( better )
                {
                    cameFrom->At( y->x, y->y ) = x;

                    if( !runtimeGrid->Contains( y ) ) runtimeGrid->Add( y );

                    runtimeGrid->At( y->x, y->y )->g = score;
                    runtimeGrid->At( y->x, y->y )->h = Heuristic( y, endNode );
                    runtimeGrid->At( y->x, y->y )->f = runtimeGrid->At( y->x, y->y )->g + runtimeGrid->At( y->x, y->y )->h;

                    if( added ) orderedOpenSet->Push( y );
                    else orderedOpenSet->Update( y );
                }
            }
        }

        return false;
    }

protected:

    void FillNeighbors( PPNT o, std::vector<PPNT>& resultContainer )
    {
        resultContainer.resize( 8 );
        int x = o->x, y = o->y;
        resultContainer[ 0 ] = &pathNodeMap->At( x - 1, y - 1 );
        resultContainer[ 1 ] = &pathNodeMap->At( x, y - 1 );
        resultContainer[ 2 ] = &pathNodeMap->At( x + 1, y - 1 );
        resultContainer[ 3 ] = &pathNodeMap->At( x - 1, y );
        resultContainer[ 4 ] = &pathNodeMap->At( x + 1, y );
        resultContainer[ 5 ] = &pathNodeMap->At( x - 1, y + 1 );
        resultContainer[ 6 ] = &pathNodeMap->At( x, y + 1 );
        resultContainer[ 7 ] = &pathNodeMap->At( x + 1, y + 1 );

        // todo: custom neighbors like teleport door ?
    }

    void ReconstructPath( PPNT n, std::vector<T*>& resultContainer )
    {
        resultContainer.clear();
        ReconstructPathRecursive( n, resultContainer );
        resultContainer.push_back( n->userContext );
    }

    void ReconstructPathRecursive( PPNT n, std::vector<T*>& resultContainer )
    {
        if( auto p = cameFrom->At( n->x, n->y ) )
        {
            ReconstructPathRecursive( p, resultContainer );
        }
        resultContainer.push_back( n->userContext );
    }

    const double sqrt_2 = sqrt( 2 );
    double Heuristic( PPNT a, PPNT b )
    {
        return sqrt( ( a->x - b->x ) * ( a->x - b->x ) + ( a->y - b->y ) * ( a->y - b->y ) );
    }
    double NeighborDistance( PPNT a, PPNT b )
    {
        int diffX = abs( a->x - b->x );
        int diffY = abs( a->y - b->y );

        switch( diffX + diffY )
        {
        case 1: return 1;
        case 2: return sqrt_2;
        case 0: return 0;
        default:
            assert( false );
            return 0;
        }
    }

};


#endif
