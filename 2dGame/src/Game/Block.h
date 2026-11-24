#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

#define BLOCK_SIZE 16

constexpr float BLOCK_BUMP_TIME   = 120.0f;   // ms: nảy lên rồi về chỗ cũ
constexpr float BLOCK_BUMP_HEIGHT = 5.0f;     // px: nảy cao nhất

// Lớp cha cho các khối đội được từ dưới lên (khối "?", gạch).
// Lo phần chung: chặn va chạm như gạch, hiệu ứng nảy, vẽ animation.
class CBlock : public CGameObject
{
protected:
    CAnimator animator;
    float bumpTimer = 0;            // > 0: đang nảy

    void  StartBump() { bumpTimer = BLOCK_BUMP_TIME; }
    bool  IsBumping() { return bumpTimer > 0; }
    float BumpOffset();             // độ cao đang nảy (px), bạn viết trong Block.cpp

public:
    CBlock(float x, float y) : CGameObject(x, y) {}

    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};
