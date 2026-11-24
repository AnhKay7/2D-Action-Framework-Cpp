#include "GrowItem.h"
#include "Engine/Collision.h"
#include "Game/AssetIDs.h"
#include "Game/Player.h"

CGrowItem::CGrowItem(float x, float y) : CGameObject(x, y)
{
    emergeTargetY = y - 16;
    nx = 1;                       // ra khỏi khối thì đi sang phải
    SetState(GROW_STATE_EMERGE);
    animator.Play(ID_ANI_GROW_ITEM);
}

void CGrowItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - 8; r = x + 8;
    t = y - 8; b = y + 8;
}

void CGrowItem::Update(float dt, std::vector<CGameObject*>* coObjects)
{
    if (state == GROW_STATE_EMERGE)
    {
        if (y > emergeTargetY) 
            y -= GROW_EMERGE_SPEED * dt;
        if (y <= emergeTargetY) 
        {
            y = emergeTargetY;
            SetState(GROW_STATE_WALK);
        }
        if (state == GROW_STATE_EMERGE)
            return;
    }

    vx = nx * GROW_WALK_SPEED;
    vy += GROW_GRAVITY * dt;
    CCollision::Process(this, dt, coObjects);
    animator.Update(dt);
}

void CGrowItem::OnCollisionWith(CCollisionEvent* e)
{
    if (e->ny != 0)
        vy = 0;
    if (e->nx != 0)
        nx = -nx;
}

void CGrowItem::OnPlayerTouch(CGameObject* player)
{
    if (state != GROW_STATE_WALK)
        return;

    CPlayer* p = dynamic_cast<CPlayer*>(player);
    if (p != nullptr)
    {
        p->Grow();
        this->Delete();
    }
}
