#pragma once
#include "Game/Enemy.h"
#include "Game/AssetIDs.h"

constexpr float WALKER_FLAT_TIME = 400.0f;      // ms nằm bẹp trước khi biến mất

#define WALKER_STATE_WALK 100
#define WALKER_STATE_FLAT 200                   // bị đạp bẹp

// Quái đi bộ: đi ngang, bị đạp thì bẹp rồi biến mất, chạm ngang thì làm player đau.
class CWalker : public CEnemy
{
    float flatTimer = 0;

protected:
    float BBoxH() override { return state == WALKER_STATE_FLAT ? 8.0f : 14.0f; }
    int   KnockAniId() override { return ID_ANI_WALKER_KNOCK; }

public:
    CWalker(float x, float y) : CEnemy(x, y) { SetState(WALKER_STATE_WALK); }

    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override;
    void Render() override { animator.Render(x, y); }
    void SetState(int state) override;

    bool IsAlive() override { return !knockedOut && state != WALKER_STATE_FLAT; }

    void OnPlayerTouch(CGameObject* player) override;   // bạn viết
};
