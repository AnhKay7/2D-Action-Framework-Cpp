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
    // TODO (bạn viết)
    // CPlayer* p = dynamic_cast<CPlayer*>(by);
    // - p là player TO: Break()
    // - ngược lại: StartBump() (chỉ khi chưa đang nảy)
}

void CBreakableBrick::Break()
{
    // TODO (bạn viết)
    // - Delete() viên gạch (collider của nó biến mất cuối frame).
    // - Spawn 4 CBrickPiece ở 4 góc viên gạch (lệch tâm ±4 px),
    //   2 mảnh bên trái bay sang trái (dir = -1), 2 mảnh bên phải sang phải (dir = 1),
    //   2 mảnh trên bay cao hơn (high = true).
}
