#include "ShellEnemy.h"
#include "Engine/Collision.h"
#include "Game/Player.h"

// ===================== Copy sẵn =====================

void CShellEnemy::SetState(int newState)
{
    CGameObject::SetState(newState);
    switch (newState)
    {
    case SHELL_STATE_IDLE:
        vx = 0;
        idleTimer = SHELL_WAKE_TIME;
        animator.Play(ID_ANI_SHELL_IDLE);
        break;
    case SHELL_STATE_SLIDE:
        graceTimer = SHELL_KICK_GRACE;
        animator.Play(ID_ANI_SHELL_SPIN);
        break;
    default:
        animator.Play(ID_ANI_SHELLW_WALK);
        break;
    }
}

void CShellEnemy::Render()
{
    // Sprite đi bộ ô 16x28: vẽ lùi lên 6 px để đáy sprite vẫn nằm ở y + 8
    bool walking = (state == SHELL_STATE_WALK && !knockedOut);
    animator.Render(x, walking ? y - 6 : y, walking && nx < 0);
}

// ===================== Phần bạn viết =====================

void CShellEnemy::Update(float dt, std::vector<CGameObject*>* coObjects)
{
    // TODO (bạn viết), xem mẫu CWalker::Update
    // 1. if (!UpdateCommon(dt)) return;
    // 2. graceTimer > 0 thì trừ dt.
    // 3. Theo state:
    //    WALK : UpdateMovement với ENEMY_WALK_SPEED, CheckEnemyContacts, quay đầu ở mép nếu turnAtEdge
    //    IDLE : UpdateMovement với tốc độ 0 (vẫn rơi được nếu đất dưới bị vỡ)
    //           idleTimer -= dt. Khi idleTimer < SHELL_WAKE_WARN: animator.Play(ID_ANI_SHELL_WAKE)
    //           Hết giờ (<= 0): SetState(SHELL_STATE_WALK)
    //    SLIDE: UpdateMovement với SHELL_SLIDE_SPEED, rồi HitEnemiesWhileSliding
    //           (gặp tường tự quay đầu nhờ OnCollisionWith của CEnemy)
    // 4. animator.Update(dt)

    if (!UpdateCommon(dt))
        return;

    if (graceTimer > 0)
        graceTimer -= dt;

    if (state == SHELL_STATE_WALK)
    {
        UpdateMovement(dt, ENEMY_WALK_SPEED, coObjects);
        CheckEnemyContacts(coObjects);
        if (!HasGroundAhead(coObjects) && turnAtEdge && isOnGround)
            nx = -nx;
    }
    else if (state == SHELL_STATE_IDLE)
    {
        UpdateMovement(dt, 0, coObjects);
        if (idleTimer > 0)
            idleTimer -= dt;
        if (idleTimer < SHELL_WAKE_WARN) 
            animator.Play(ID_ANI_SHELL_WAKE);
        if (idleTimer <= 0)
            SetState(SHELL_STATE_WALK);
    }
    else if (state == SHELL_STATE_SLIDE) {
        UpdateMovement(dt, SHELL_SLIDE_SPEED, coObjects);
        HitEnemiesWhileSliding(coObjects);
    }

    animator.Update(dt);
}

void CShellEnemy::Kick(int dir)
{
    nx = dir;
    SetState(SHELL_STATE_SLIDE);
}

void CShellEnemy::HitEnemiesWhileSliding(std::vector<CGameObject*>* coObjects)
{
    // TODO (bạn viết)
    // Duyệt coObjects, dynamic_cast<CEnemy*>. Bỏ qua: nullptr, chính mình, đã xoá, !IsAlive().
    // Bbox chồng lên nhau: other->KnockOut(nx) (văng theo hướng mai đang trượt).
    // (Tuỳ chọn) Nếu other cũng là mai đang trượt thì cả hai cùng văng: thêm KnockOut(-nx).
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);
    for (CGameObject* obj : *coObjects)
    {
        CEnemy* emy = dynamic_cast<CEnemy*>(obj);
        if (emy == nullptr || obj == this || obj->IsDeleted())
            continue;

        if (!emy->IsAlive() || emy->IsKnockedOut())
            continue;

        float el, et, er, eb;
        emy->GetBoundingBox(el, et, er, eb);

        if (CCollision::Overlap(l, t, r, b, el, et, er, eb))
        {
            emy->KnockOut(nx);
        }
    }
}

void CShellEnemy::OnPlayerTouch(CGameObject* player)
{
    CPlayer* p = dynamic_cast<CPlayer*>(player);
    if (p == nullptr || knockedOut)
        return;
    if (state == SHELL_STATE_WALK)
    {
        if (IsStompedBy(p))
        {
            SetState(SHELL_STATE_IDLE);
            p->OnStompEnemy();
        }
        else
            p->Hurt();
    }
    else if (state == SHELL_STATE_IDLE)
    {
        if (graceTimer > 0) 
        { 
            graceTimer = SHELL_KICK_GRACE; 
            return; 
        }
        int dir = x - p->GetX() > 0 ? 1 : -1;
        Kick(dir);
        if (IsStompedBy(p))
            p->OnStompEnemy();
    }
    else if (state == SHELL_STATE_SLIDE)
    {
        if (graceTimer > 0) 
        {
            graceTimer = SHELL_KICK_GRACE;
            return;
        }

        if (IsStompedBy(p))
        {
            SetState(SHELL_STATE_IDLE);
            graceTimer = SHELL_KICK_GRACE;
            p->OnStompEnemy();
        }
        else
            p->Hurt();
    }
}
