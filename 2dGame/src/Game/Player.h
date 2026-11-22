#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"
#include "Game/PlayerStats.h"

#define PLAYER_STATE_IDLE   100
#define PLAYER_STATE_WALK   200
#define PLAYER_STATE_RUN    210     
#define PLAYER_STATE_SKID   220     
#define PLAYER_STATE_JUMP   400
#define PLAYER_STATE_FALL   500

#define KEY_JUMP        'Z'
#define KEY_RUN         'X'         
#define KEY_MOVE_LEFT   VK_LEFT
#define KEY_MOVE_RIGHT  VK_RIGHT

struct PlayerInput
{
    int  moveDir = 0;               
    bool jumpPressed = false;
    bool jumpHeld = false;
    bool runHeld = false;
};

class CPlayer : public CGameObject
{
private:
    CAnimator animator;
    PlayerInput in;
    bool inputEnabled = true;

    bool  isOnGround = false;
    bool  isSkidding = false;
    float meter = 0;                

    float coyoteTimer = 0;
    float jumpBufferTimer = 0;

    void ReadInput();
    void CheckSensors(std::vector<CGameObject*>* coObjects);
    void UpdateHorizontal(float dt);
    void UpdateVertical(float dt);
    void UpdateMeter(float dt);
    void UpdateState();

    float JumpSpeedForCurrentRun();

public:
    CPlayer(float x, float y) : CGameObject(x, y) { SetState(PLAYER_STATE_IDLE); }

    void Update(float dt, std::vector<CGameObject*>* coObjects) override;
    void Render() override;
    void SetState(int state) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnCollisionWith(CCollisionEvent* e) override;

    bool  IsOnGround() { return isOnGround; }
    bool  IsMeterFull() { return meter >= PlayerStats::METER_MAX; }
    float GetMeter() { return meter; }
    float GetVx() { return vx; }
    float GetVy() { return vy; }
};