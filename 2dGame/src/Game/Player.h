#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

#define PLAYER_STATE_IDLE    100
#define PLAYER_STATE_RUN     200
#define PLAYER_STATE_ATTACK  300

#define PLAYER_RUN_SPEED   0.08f   // px / ms

#define ID_TEX_IDLE    10
#define ID_TEX_RUN     11
#define ID_TEX_ATTACK1 12

#define ID_SPRITE_IDLE    1000   // 1000..1006
#define ID_SPRITE_RUN     1100   // 1100..1107
#define ID_SPRITE_ATTACK1 1200   // 1200..1209

#define ID_ANI_IDLE    1
#define ID_ANI_RUN     2
#define ID_ANI_ATTACK1 3

#define PATH_TEX_PLAYER_IDLE L"assets\\player\\Idle.png"
#define PATH_TEX_PLAYER_RUN L"assets\\player\\Run.png"
#define PATH_TEX_PLAYER_ATTACK1 L"assets\\player\\Attack1.png"

// Hộp va chạm của THÂN (không tính kiếm, không tính phần trong suốt)
#define PLAYER_BBOX_WIDTH  12
#define PLAYER_BBOX_HEIGHT 20

#define PLAYER_FEET_OFFSET 8 

class CPlayer : public CGameObject      // giống "class CPlayer : CGameObject" bên C#
{
    CAnimator animator;

    void HandleInput();                 // đọc phím → quyết định đổi state

public:
    CPlayer(float x, float y);

    void Update(int dt) override;
    void Render() override;
    void SetState(int state) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};