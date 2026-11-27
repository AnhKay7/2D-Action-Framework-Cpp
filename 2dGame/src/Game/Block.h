#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"

#define BLOCK_SIZE 16

constexpr float BLOCK_BUMP_TIME   = 120.0f;
constexpr float BLOCK_BUMP_HEIGHT = 5.0f;

class CBlock : public CGameObject
{
protected:
    CAnimator animator;
    float bumpTimer = 0;           

    void  StartBump() { bumpTimer = BLOCK_BUMP_TIME; HitObjectsAbove(); }
    void  HitObjectsAbove();        
    bool  IsBumping() { return bumpTimer > 0; }
    float BumpOffset();             

public:
    CBlock(float x, float y) : CGameObject(x, y) {}

    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};
