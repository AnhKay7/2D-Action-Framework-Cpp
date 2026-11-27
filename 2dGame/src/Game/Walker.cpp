#include "Walker.h"
#include "Game/Player.h"

void CWalker::SetState(int newState)
{
    CGameObject::SetState(newState);
    if (newState == WALKER_STATE_FLAT)
    {
        vx = 0;
        flatTimer = WALKER_FLAT_TIME;
        animator.Play(ID_ANI_WALKER_FLAT);
    }
    else
        animator.Play(ID_ANI_WALKER_WALK);
}

// Mẫu Update cho mọi quái (đã viết sẵn, đọc để hiểu thứ tự rồi làm theo cho CShellEnemy)
void CWalker::Update(float dt, std::vector<CGameObject*>* coObjects)
{
    if (!UpdateCommon(dt))
        return;

    if (state == WALKER_STATE_FLAT)
    {
        flatTimer -= dt;
        if (flatTimer <= 0)
            Delete();
        animator.Update(dt);
        return;
    }

    UpdateMovement(dt, ENEMY_WALK_SPEED, coObjects);
    CheckEnemyContacts(coObjects);
    if (turnAtEdge && isOnGround && !HasGroundAhead(coObjects))
        nx = -nx;

    animator.Update(dt);
}

void CWalker::OnPlayerTouch(CGameObject* player)
{
    CPlayer* p = dynamic_cast<CPlayer*>(player);
    if (p == nullptr || !IsAlive())
        return;
    if (IsStompedBy(p))
    {
        SetState(WALKER_STATE_FLAT);
        p->OnStompEnemy();
    }
    else
        p->Hurt();

}
