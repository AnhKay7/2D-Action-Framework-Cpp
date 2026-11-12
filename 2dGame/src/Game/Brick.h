#pragma once
#include "Engine/GameObject.h"

#define BRICK_SIZE 16

class CBrick : public CGameObject
{
public:
    CBrick(float x, float y) : CGameObject(x, y) {}   
                 
    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override {};
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};