#pragma once
#include <vector>

class CGameObject;

#define BLOCK_PUSH 0.01f

struct CCollisionEvent
{
    CGameObject* obj = nullptr;  
    float t = 2.0f;               
    float nx = 0, ny = 0;        
};

class CCollision
{
private:
    static bool FindFirstHit(CGameObject* src, float dx, float dy,
        std::vector<CGameObject*>* coObjects, CCollisionEvent& hit);

public:
    static void SweptAABB(
        float ml, float mt, float mr, float mb,
        float dx, float dy,
        float sl, float st, float sr, float sb,
        float& t, float& nx, float& ny);

    static void Process(CGameObject* src, float dt, std::vector<CGameObject*>* coObjects);

    static bool Overlap(float l1, float t1, float r1, float b1,
        float l2, float t2, float r2, float b2)
    {
        return l1 < r2 && r1 > l2 && t1 < b2 && b1 > t2;
    }
};