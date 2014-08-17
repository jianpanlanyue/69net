﻿#include "Lib/All.h"
#include "PacketTestPackets.h"

namespace PacketTestPackets
{
    ushort Obj_t1::getTypeID()
    {
        return 0;
    }
    void Obj_t1::fill( FlatBuffer& fb
        , int pI_ )
    {
        fb.writes(
            pI_ );
    }

    void Obj_t1::writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes(
            _i_ );
    }

    bool Obj_t1::readBuffer( FlatBuffer& fb )
    {
        return fb.reads(
            _i_ );
    }

    void Obj_t1::writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.writesDirect(
            _i_ );
    }

    int Obj_t1::getWriteBufferSize() const
    {
        return 
            sizeof( _i_ );
    }

    ushort Obj_t2::getTypeID()
    {
        return 1;
    }
    void Obj_t2::fill( FlatBuffer& fb
        , ::PacketTestPackets::Obj_t1( &pTl )[12] )
    {
        fb.writes(
            pTl );
    }

    void Obj_t2::writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes(
            _tl );
    }

    bool Obj_t2::readBuffer( FlatBuffer& fb )
    {
        return fb.reads(
            _tl );
    }

    void Obj_t2::writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.writesDirect(
            _tl );
    }

    int Obj_t2::getWriteBufferSize() const
    {
        return 
            BufferUtils::getSizes( _tl );
    }

    ushort Obj_t3::getTypeID()
    {
        return 2;
    }
    void Obj_t3::fill( FlatBuffer& fb
        , ::PacketTestPackets::Obj_t2( &pTl )[12] )
    {
        fb.writes(
            pTl );
    }

    void Obj_t3::writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes(
            _tl );
    }

    bool Obj_t3::readBuffer( FlatBuffer& fb )
    {
        return fb.reads(
            _tl );
    }

    void Obj_t3::writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.writesDirect(
            _tl );
    }

    int Obj_t3::getWriteBufferSize() const
    {
        return 
            BufferUtils::getSizes( _tl );
    }

}
