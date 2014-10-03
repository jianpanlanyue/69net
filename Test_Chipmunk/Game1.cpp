#include "Precompile.h"

/*
设计尺寸：    768 * 1024
格子划分：    12 * 16
每格尺寸：    64 * 64
细胞尺寸：    32 * 32 ( 即 1/4 格 )
*/

Game1::Game1()
{
    int dw = 768, dh = 1024;
    G::scene->designSize = { (float)dw, (float)dh };
    G::window->Init( L"test", dw, dh );
    //G::window->setVsync( false );

    auto mb = new MarginBox();
    ScopeGuard mbSG = [ &] { mb->Release(); };
    mb->margin = { 5, 5, 5, 5 };
    G::scene->Add( mb );

    auto title = new Box();
    title->AutoRelease();
    title->color = { 0, 255, 0, 0 };
    title->dock = { 0.5, 1 };
    title->anchor = { 0.5, 1 };
    title->size = { 300, 30 };
    title->offset = { 0, -1 };
    mb->Add( title );

    auto close = Box::Create();
    close->color = { 255, 0, 0, 0 };
    close->dock = { 1, 1 };
    close->anchor = { 1, 1 };
    close->size = { 30, 30 };
    close->offset = { -1, -1 };
    mb->Add( close );

    auto box2 = Box::Create();
    box2->size = { 200, 200 };
    box2->offset = { 401, 200 };
    mb->Add( box2 );
}

void Game1::Update()
{
    // todo: loop code here
}

Game1::~Game1()
{
}
