#pragma once
#include "Game/Enemy.h"
#include "Game/AssetIDs.h"

constexpr float SHELL_SLIDE_SPEED = 0.20f;      // (3.3 px/frame) nhanh hơn player chạy hết tốc
constexpr float SHELL_WAKE_TIME   = 5000.0f;    // ms nằm trong mai trước khi chui ra
constexpr float SHELL_WAKE_WARN   = 1500.0f;    // 1.5 s cuối: đổi animation báo sắp chui ra
constexpr float SHELL_KICK_GRACE  = 150.0f;     // vừa đá xong: mai không làm đau player

#define SHELL_STATE_WALK  100       // đi bộ (sprite cao 16x24)
#define SHELL_STATE_IDLE  200       // rụt trong mai, nằm yên
#define SHELL_STATE_SLIDE 300       // mai đang trượt

// Quái có mai: bị đạp thì rụt vào mai, đá mai thì mai trượt nhanh,
// dọn quái trên đường, gặp tường thì dội lại. Nằm yên lâu thì chui ra đi tiếp.
class CShellEnemy : public CEnemy
{
    float idleTimer = 0;
    float graceTimer = 0;

    void Kick(int dir);                                                 // bạn viết
    void HitEnemiesWhileSliding(std::vector<CGameObject*>* coObjects);  // bạn viết

protected:
    float BBoxH() override { return state == SHELL_STATE_WALK ? 22.0f : 14.0f; }
    int   KnockAniId() override { return ID_ANI_SHELL_KNOCK; }

public:
    CShellEnemy(float x, float y) : CEnemy(x, y) { SetState(SHELL_STATE_WALK); }

    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override;  // bạn viết
    void Render() override;
    void SetState(int state) override;

    void OnPlayerTouch(CGameObject* player) override;   // bạn viết
};
