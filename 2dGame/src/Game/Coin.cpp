#include "Coin.h"
#include "Game/AssetIDs.h"
#include "Game/Player.h"

CCoin::CCoin(float x, float y) : CGameObject(x, y)
{
    animator.Play(ID_ANI_COIN);
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - 6; r = x + 6;
    t = y - 7; b = y + 7;
}

void CCoin::OnPlayerTouch(CGameObject* player)
{
    CPlayer* p = dynamic_cast<CPlayer*>(player);
    if (p != nullptr)
        p->AddCoin();
    Delete();
}
