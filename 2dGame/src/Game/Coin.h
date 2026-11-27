#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

// Xu đặt sẵn trong màn: không chặn, player chạm vào là nhặt
class CCoin : public CGameObject
{
    CAnimator animator;

public:
    CCoin(float x, float y);

    int IsBlock() override { return 0; }

    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override { animator.Update(dt); }
    void Render() override { animator.Render(x, y); }
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    void OnPlayerTouch(CGameObject* player) override;   // bạn viết
};
