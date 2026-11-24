#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

constexpr float GROW_EMERGE_SPEED = 0.027f;   // px/ms: chui lên 16 px mất khoảng 600 ms
constexpr float GROW_WALK_SPEED   = 0.05f;    // px/ms
constexpr float GROW_GRAVITY      = 0.001f;
constexpr float GROW_MAX_FALL     = 0.24f;

#define GROW_STATE_EMERGE 100     // đang chui ra khỏi khối, chưa va chạm
#define GROW_STATE_WALK   200     // đi ngang, rơi, gặp tường quay đầu

// Vật phẩm biến to
class CGrowItem : public CGameObject
{
    CAnimator animator;
    float emergeTargetY;          // y khi đã chui ra hẳn (trên khối 16 px)

public:
    CGrowItem(float x, float y);

    int IsBlock() override { return 0; }

    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override;  // bạn viết
    void Render() override { animator.Render(x, y); }
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnCollisionWith(CCollisionEvent* e) override;                               // bạn viết
    void OnPlayerTouch(CGameObject* player) override;                                // bạn viết
};
