#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

class CPlayer;

// ----- Thông số chung của quái (px, ms) -----
constexpr float ENEMY_GRAVITY       = 0.001f;
constexpr float ENEMY_MAX_FALL      = 0.24f;
constexpr float ENEMY_WALK_SPEED    = 0.03f;    // (0.5 px/frame)
constexpr float ENEMY_WAKE_MARGIN   = 32.0f;    // cách mép màn hình 2 ô thì quái thức dậy
constexpr float ENEMY_FORGET_MARGIN = 160.0f;   // ra xa mép màn hình hơn mức này thì bị xoá
constexpr float ENEMY_KNOCK_VY      = -0.25f;   // bị hất văng: bật lên...
constexpr float ENEMY_KNOCK_VX      = 0.04f;    // ...và trôi ngang
constexpr float STOMP_TOLERANCE     = 6.0f;     // chân player thấp hơn đỉnh quái tối đa 6 px vẫn tính là đạp

// Lớp cha của mọi quái.
// Quy ước vị trí: x = giữa, y = tâm ô 16x16 dưới chân, tức là CHÂN quái nằm ở y + 8.
// Bbox luôn bám đáy (y + 8), nên đổi chiều cao bbox (đi bộ -> rụt vào mai) quái vẫn đứng trên đất.
class CEnemy : public CGameObject
{
protected:
    CAnimator animator;
    bool  active = false;       // false: đang ngủ, chờ camera tới gần
    bool  knockedOut = false;   // true: đã chết kiểu bị hất văng, rơi xuyên mọi thứ
    bool  turnAtEdge = false;   // true: tới mép bục thì quay đầu (thuộc tính TurnAtEdge trong Tiled)
    bool  isOnGround = false;   // UpdateMovement đặt false, OnCollisionWith đặt true
    float fallLimitY = 100000.0f;

    // Kích thước bbox theo state hiện tại, lớp con override
    virtual float BBoxW() { return 14; }
    virtual float BBoxH() { return 14; }
    virtual int   KnockAniId() = 0;         // animation khi bị hất văng

    bool UpdateCommon(float dt);            // (copy sẵn) thức/ngủ, rơi khi bị hất, xoá khi ra xa. false = khỏi làm gì thêm
    bool IsNearCamera(float margin);        // (copy sẵn)

    void UpdateMovement(float dt, float speed, std::vector<CGameObject*>* coObjects);  // bạn viết
    void CheckEnemyContacts(std::vector<CGameObject*>* coObjects);                      // bạn viết
    bool HasGroundAhead(std::vector<CGameObject*>* coObjects);                          // bạn viết (tuỳ chọn)
    bool IsStompedBy(CPlayer* player);                                                  // bạn viết

public:
    CEnemy(float x, float y);

    int  IsBlock() override { return 0; }
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnCollisionWith(CCollisionEvent* e) override;      // bạn viết
    void OnHitFromBelow(CGameObject* by) override;          // (copy sẵn) khối dưới chân bị đội: bị hất văng

    virtual void KnockOut(int dir);         // (copy sẵn) chết kiểu bị hất văng, dir = hướng văng
    virtual bool IsAlive() { return !knockedOut; }   // còn "sống" để chặn/va chạm với quái khác

    bool IsKnockedOut() { return knockedOut; }
    bool IsActive()     { return active; }
    int  GetDir()       { return nx; }
    void SetTurnAtEdge(bool v) { turnAtEdge = v; }
};
