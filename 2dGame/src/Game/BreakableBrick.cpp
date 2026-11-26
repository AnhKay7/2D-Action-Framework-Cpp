#include "BreakableBrick.h"
#include "Game/AssetIDs.h"
#include "Game/PlayScene.h"
#include "Game/Player.h"
#include "Game/BrickPiece.h"

CBreakableBrick::CBreakableBrick(float x, float y) : CBlock(x, y)
{
    animator.Play(ID_ANI_BRICK);
}

void CBreakableBrick::OnHitFromBelow(CGameObject* by)
{
    CPlayer* p = dynamic_cast<CPlayer*>(by);
    if (p != nullptr)
    {
        if (p->IsBig())
            Break();
        else
            StartBump();
    }
}

void CBreakableBrick::Break()
{
    Delete();
    CPlayScene::current->Spawn(new CBrickPiece(x - 4, y - 4, -1, true), true);
    CPlayScene::current->Spawn(new CBrickPiece(x + 4, y - 4, 1, true), true);
    CPlayScene::current->Spawn(new CBrickPiece(x - 4, y + 4, -1, false));
    CPlayScene::current->Spawn(new CBrickPiece(x + 4, y + 4, 1, false));
}
