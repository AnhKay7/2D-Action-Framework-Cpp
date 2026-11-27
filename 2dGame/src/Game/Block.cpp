#include "Block.h"
#include "Game/PlayScene.h"
#include "Engine/Collision.h"

void CBlock::Update(float dt, std::vector<CGameObject*>* coObjects)
{
    if (bumpTimer > 0)
    {
        bumpTimer -= dt;
        if (bumpTimer < 0)
            bumpTimer = 0;
    }
    animator.Update(dt);
}

void CBlock::Render()
{
    animator.Render(x, y - BumpOffset());
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - BLOCK_SIZE / 2;
    t = y - BLOCK_SIZE / 2;
    r = l + BLOCK_SIZE;
    b = t + BLOCK_SIZE;
}

float CBlock::BumpOffset()
{
    if (bumpTimer <= 0)
        return 0;
    float halfBumpTime = BLOCK_BUMP_TIME / 2;;
    float percent = 1;
    if (bumpTimer >= halfBumpTime)
        percent = (BLOCK_BUMP_TIME - bumpTimer) / halfBumpTime;
    else
        percent = bumpTimer / halfBumpTime;
    return percent * BLOCK_BUMP_HEIGHT;
}

void CBlock::HitObjectsAbove()
{
    if (CPlayScene::current == nullptr)
        return;
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    for (CGameObject* obj : CPlayScene::current->GetObjects())
    {
        if (obj == this || obj->IsBlock() || obj->IsDeleted())
            continue;

        float el, et, er, eb;
        obj->GetBoundingBox(el, et, er, eb);

        if (CCollision::Overlap(l, t - 2, r, t, el, et, er, eb))
            obj->OnHitFromBelow(this);
    }
}