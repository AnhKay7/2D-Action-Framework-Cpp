#include "Enemy.h"
#include "Engine/Game.h"
#include "Engine/Collision.h"
#include "Engine/Debug.h"
#include "Game/Player.h"
#include "Game/PlayScene.h"

CEnemy::CEnemy(float x, float y) : CGameObject(x, y)
{
    nx = -1;    // mặc định đi sang trái, về phía player đang chạy tới
    if (CPlayScene::current != nullptr)
        fallLimitY = CPlayScene::current->GetMapHeightPx() + 32.0f;
}

void CEnemy::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    b = y + 8;
    t = b - BBoxH();
    l = x - BBoxW() / 2;
    r = l + BBoxW();
}

// ===================== Copy sẵn, không cần sửa =====================

bool CEnemy::IsNearCamera(float margin)
{
    CGame* game = CGame::GetInstance();
    float camX, camY;
    game->GetCamPos(camX, camY);
    float viewW = (float)game->GetBackBufferWidth();
    return x > camX - margin && x < camX + viewW + margin;
}

bool CEnemy::UpdateCommon(float dt)
{
    // 1. Ngủ cho tới khi camera tới gần. Nhờ vậy quái không tự đi lạc trước khi player thấy nó.
    if (!active)
    {
        if (!IsNearCamera(ENEMY_WAKE_MARGIN))
            return false;
        active = true;
    }

    // 2. Đã bị hất văng: chỉ rơi, xuyên qua mọi thứ, rơi khỏi map thì xoá
    if (knockedOut)
    {
        vy += ENEMY_GRAVITY * dt;
        x += vx * dt;
        y += vy * dt;
        if (y > fallLimitY)
            Delete();
        animator.Update(dt);
        return false;
    }

    // 3. Rơi xuống hố hoặc đi quá xa khỏi màn hình: xoá
    if (y > fallLimitY || !IsNearCamera(ENEMY_FORGET_MARGIN))
    {
        Delete();
        return false;
    }
    return true;
}

void CEnemy::KnockOut(int dir)
{
    if (knockedOut)
        return;
    knockedOut = true;
    active = true;
    vx = dir * ENEMY_KNOCK_VX;
    vy = ENEMY_KNOCK_VY;
    animator.Play(KnockAniId(), true);
    DebugOut(L"[ENEMY] Bi hat vang\n");
}

void CEnemy::OnHitFromBelow(CGameObject* by)
{
    if (knockedOut)
        return;
    KnockOut(x < by->GetX() ? -1 : 1);     // văng ra phía xa tâm khối
}

// ===================== Phần bạn viết =====================

void CEnemy::UpdateMovement(float dt, float speed, std::vector<CGameObject*>* coObjects)
{
    vx = nx * speed;
    vy += ENEMY_GRAVITY * dt;
    if (vy > ENEMY_MAX_FALL)
        vy = ENEMY_MAX_FALL;
    isOnGround = false;
    CCollision::Process(this, dt, coObjects);
}

void CEnemy::OnCollisionWith(CCollisionEvent* e)
{
    if (e->ny != 0)
        vy = 0;
    if (e->ny < 0)
        isOnGround = true;
    if (e->nx != 0)
        nx = -nx;
}

void CEnemy::CheckEnemyContacts(std::vector<CGameObject*>* coObjects)
{
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    for (CGameObject* obj : *coObjects)
    {
        CEnemy* emy = dynamic_cast<CEnemy*>(obj);
        if (emy == nullptr || emy == this || obj->IsDeleted())
            continue;

        if (!emy->IsAlive())
            continue;

        float el, et, er, eb;
        emy->GetBoundingBox(el, et, er, eb);

        if (CCollision::Overlap(l, t, r, b, el, et, er, eb))
        {
            if ((emy->GetX() - x) * nx > 0)
                nx = -nx;
        }
    }
}

bool CEnemy::HasGroundAhead(std::vector<CGameObject*>* coObjects)
{
    float px = x + nx * (BBoxW() / 2 + 1);
    float py = y + 8 + 2;

    for (CGameObject* obj : *coObjects)
    {
        if (obj->IsDeleted() || !obj->IsBlock())
            continue;

        float l, t, r, b;
        obj->GetBoundingBox(l, t, r, b);
        if (CCollision::Overlap(px - 1, py - 1, px + 1, py + 1, l, t, r, b))
            return true;
    }
    return false;
}

bool CEnemy::IsStompedBy(CPlayer* player)
{
    float pl, pt, pr, pb;
    player->GetBoundingBox(pl, pt, pr, pb);
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);
    if (player->GetVy() > 0 && pb <= t + STOMP_TOLERANCE)
        return true;
    return false;
}
