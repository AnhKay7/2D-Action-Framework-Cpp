#include "CoinPop.h"
#include "Game/AssetIDs.h"

CCoinPop::CCoinPop(float x, float y) : CGameObject(x, y), startY(y)
{
    vy = -COIN_POP_SPEED;
    animator.Play(ID_ANI_COIN_POP);
}

void CCoinPop::Update(float dt, std::vector<CGameObject*>* coObjects)
{
    vy += COIN_POP_GRAVITY * dt;
    y += vy * dt;
    if (vy > 0 && y > startY)
        Delete();
    animator.Update(dt);
}
