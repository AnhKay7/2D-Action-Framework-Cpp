#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

#define PLAYER_STATE_IDLE       100
#define PLAYER_STATE_RUN        200
#define PLAYER_STATE_ATTACK     300
#define PLAYER_STATE_JUMP       400
#define PLAYER_STATE_FALL       500

#define PLAYER_RUN_SPEED        0.08f   
#define PLAYER_JUMP_SPEED       0.23f // v^2 = 2 * g * h (g = 0.0005, h = 53)
#define PLAYER_JUMP_CUT         0.35f 

#define PLAYER_BBOX_WIDTH  12
#define PLAYER_BBOX_HEIGHT 20

#define PLAYER_FEET_OFFSET 8

#define PLAYER_GRAVITY         0.0005f   // px/ms², ≈ 9.81 * 5 unit/s² bên Unity
#define PLAYER_MAX_FALL_SPEED  0.2f      // px/ms,  ≈ 20 unit/s bên Unity

#define PLAYER_COYOTE_TIME       150.0f
#define PLAYER_JUMP_BUFFER_TIME  150.0f


class CPlayer : public CGameObject     
{
private:
    CAnimator animator;

    void HandleInput();                

    bool isOnGround = false;

    float coyoteTimer = 0;
    float jumpBufferTimer = 0;

public:
    CPlayer(float x, float y);

    void Update(float dt, std::vector<CGameObject*>* coObjects) override;
    void Render() override;
    void SetState(int state) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    void OnCollisionWith(CCollisionEvent* e) override;

    bool IsOnGround() { return isOnGround; }
    float GetVy() { return vy; }
    void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }
};