#pragma once
#include "Engine/GameObject.h"

#define BRICK_SIZE 16

class CBrick : public CGameObject
{
public:
    CBrick(float x, float y) : CGameObject(x, y) {}   // gọi constructor lớp cha, giống ": base(x, y)" bên C#

    void Update(int dt) override {}                   // gạch đứng yên, không làm gì
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};