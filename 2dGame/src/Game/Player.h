#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"
#include "Game/PlayerStats.h"
#include "Engine/Debug.h"

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

    bool isOnGround = false;
    bool isSkidding = false;
    bool isBig = false;
    float meter = 0;                

    float coyoteTimer = 0;
    float jumpBufferTimer = 0;

    float spawnX = 0, spawnY = 0;
    float fallLimitY = 100000.0f;

    int coins = 0;

    float invulnTimer = 0;      // > 0: vừa bị thương, nhấp nháy, quái không làm đau
    int   stompChain = 0;       // số quái đạp liên tiếp chưa chạm đất
    int   score = 0;            // TẠM: GĐ7 chuyển sang CGameState

    void ReadInput();
    void CheckSensors(std::vector<CGameObject*>* coObjects);
    void UpdateHorizontal(float dt);
    void UpdateVertical(float dt);
    void UpdateMeter(float dt);
    void UpdateState();

    float JumpSpeedForCurrentRun();

    void CheckTouches(std::vector<CGameObject*>* coObjects);
public:
    CPlayer(float x, float y) : CGameObject(x, y), spawnX(x), spawnY(y) { SetState(PLAYER_STATE_IDLE); }

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

    void SetFallLimit(float y) { fallLimitY = y; }

    int  IsBlock() override { return 0; }
    bool IsBig() { return isBig; }
    void AddCoin() { coins++; }
    int  GetCoins() { return coins; }
    void Grow()
    {
        if (!isBig)
        {
            isBig = true;
            DebugOut(L"[PLAYER] Grow\n");
        }
    }

    int  GetScore() { return score; }
    bool IsInvulnerable() { return invulnTimer > 0; }

    void OnStompEnemy();        // bạn viết
    void Hurt();                // TẠM tới bước 19
    void Respawn();
};