#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

#define PLAYER_STATE_IDLE       100
#define PLAYER_STATE_RUN        200
#define PLAYER_STATE_ATTACK     300
#define PLAYER_STATE_JUMP       400
#define PLAYER_STATE_FALL       500
#define PLAYER_STATE_WALL_SLIDE   600
#define PLAYER_STATE_WALL_JUMP    700

#define PLAYER_RUN_SPEED        0.08f   
#define PLAYER_JUMP_SPEED       0.23f // v^2 = 2 * g * h (g = 0.0005, h = 53)
#define PLAYER_JUMP_CUT         0.35f 

#define PLAYER_BBOX_WIDTH  12
#define PLAYER_BBOX_HEIGHT 20

#define PLAYER_FEET_OFFSET 8

#define PLAYER_GRAVITY         0.0005f   
#define PLAYER_MAX_FALL_SPEED  0.2f      

#define PLAYER_COYOTE_TIME       150.0f
#define PLAYER_JUMP_BUFFER_TIME  150.0f

#define PLAYER_SENSOR_SKIN        1.0f     
#define PLAYER_WALL_SLIDE_SPEED   0.053f 
#define PLAYER_WALL_COYOTE_TIME   80.0f    
#define PLAYER_WALL_JUMP_SPEED    0.128f   
#define PLAYER_WALL_JUMP_LOCK     150.0f


class CPlayer : public CGameObject     
{
private:
    CAnimator animator;

    void HandleInput();                

    bool isOnGround = false;

    float coyoteTimer = 0;
    float jumpBufferTimer = 0;

    bool  isTouchingWall = false;
    int   wallDir = 0;              // 1: tường bên phải, -1: tường bên trái. GIỮ NGUYÊN khi rời tường (cho wall coyote)
    float wallCoyoteTimer = 0;
    float wallJumpTimer = 0;

public:
    CPlayer(float x, float y);

    void Update(float dt, std::vector<CGameObject*>* coObjects) override;
    void Render() override;
    void SetState(int state) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    void OnCollisionWith(CCollisionEvent* e) override;

    bool IsOnGround() { return isOnGround; }
    bool IsTouchingWall() { return isTouchingWall; }
    int GetWallDirection() { return wallDir; }
    float GetVy() { return vy; }
    void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }

    void CheckWalls(std::vector<CGameObject*>* coObjects);
};