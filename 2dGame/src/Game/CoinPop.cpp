#include "CoinPop.h"
#include "Game/AssetIDs.h"

CCoinPop::CCoinPop(float x, float y) : CGameObject(x, y), startY(y)
{
    vy = -COIN_POP_SPEED;
    animator.Play(ID_ANI_COIN_POP);
}

void CCoinPop::Update(float dt, std::vector<CGameObject*>* coObjects)
{
    // TODO (bạn viết)
    // - Cộng trọng lực vào vy, cộng vy*dt vào y (không cần CCollision, xu này xuyên mọi thứ).
    // - Khi đang RƠI (vy > 0) và y đã xuống tới startY - 8 thì Delete().
    vy += COIN_POP_GRAVITY * dt;
    if (vy > 0 && y > startY - 8)
        Delete();
    animator.Update(dt);
}
