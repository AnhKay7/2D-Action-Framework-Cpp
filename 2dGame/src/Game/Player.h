#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

#define PLAYER_STATE_IDLE    100
#define PLAYER_STATE_RUN     200
#define PLAYER_STATE_ATTACK  300

#define PLAYER_RUN_SPEED   0.08f   // px / ms

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