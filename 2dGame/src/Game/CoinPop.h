#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

constexpr float COIN_POP_SPEED   = 0.35f;     // px/ms, bật lên
constexpr float COIN_POP_GRAVITY = 0.0012f;   // px/ms²

// Xu bật ra từ khối "?": vọt lên rồi rơi lại, tới gần khối thì biến mất. Không va chạm.
class CCoinPop : public CGameObject
{
    CAnimator animator;
    float startY;

public:
    CCoinPop(float x, float y);

    int IsBlock() override { return 0; }

    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override;   // bạn viết
    void Render() override { animator.Render(x, y); }
    void GetBoundingBox(float& l, float& t, float& r, float& b) override { l = r = x; t = b = y; }
};
