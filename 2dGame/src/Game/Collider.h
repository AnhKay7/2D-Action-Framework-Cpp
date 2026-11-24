#pragma once
#include "Engine/GameObject.h"

// Khối va chạm vô hình sinh ra từ layer Solid / SemiSolid của map.
// Không vẽ gì (hình đã do CTileMap vẽ), chỉ có bounding box.
class CCollider : public CGameObject
{
    float w, h;
    bool  semiSolid;

public:
    // (l, t): góc trên-trái, w x h: kích thước, đơn vị px
    CCollider(float l, float t, float w, float h, bool semiSolid)
        : CGameObject(l + w / 2, t + h / 2), w(w), h(h), semiSolid(semiSolid) {}

    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override {}
    void Render() override {}

    void GetBoundingBox(float& l, float& t, float& r, float& b) override
    {
        l = x - w / 2;
        t = y - h / 2;
        r = l + w;
        b = t + h;
    }

    bool IsSemiSolid() override { return semiSolid; }
};
