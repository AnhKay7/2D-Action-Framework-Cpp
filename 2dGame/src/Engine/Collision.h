#pragma once
#include <vector>

class CGameObject;

#define BLOCK_PUSH 0.01f

struct CCollisionEvent
{
    CGameObject* obj = nullptr;   // vật bị chạm
    float t = 2.0f;               // lúc chạm, 0..1. Lớn hơn 1 nghĩa là chưa chạm gì
    float nx = 0, ny = 0;         // mặt bị chạm (xem bảng 9.1)
};

class CCollision
{
public:
    // Toán thuần: hộp m đi một quãng (dx, dy), hộp s đứng yên. Không chạm thì t = -1
    static void SweptAABB(
        float ml, float mt, float mr, float mb,
        float dx, float dy,
        float sl, float st, float sr, float sb,
        float& t, float& nx, float& ny);

    // Di chuyển src theo vận tốc của nó trong dt, bị chặn bởi các object IsBlock()
    static void Process(CGameObject* src, int dt, std::vector<CGameObject*>* coObjects);

private:
    // Tìm vật chạm SỚM NHẤT khi src đi quãng (dx, dy). Có thì trả true và điền hit
    static bool FindFirstHit(CGameObject* src, float dx, float dy,
        std::vector<CGameObject*>* coObjects, CCollisionEvent& hit);
};