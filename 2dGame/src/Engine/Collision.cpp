#include "Collision.h"
#include "GameObject.h"

void CCollision::SweptAABB(
    float ml, float mt, float mr, float mb,
    float dx, float dy,
    float sl, float st, float sr, float sb,
    float& t, float& nx, float& ny)
{
    t = -1.0f; nx = ny = 0;

    // 1. Lọc nhanh: hộp bao cả quãng đường mà không đụng s thì khỏi tính
    float bl = dx > 0 ? ml : ml + dx;
    float bt = dy > 0 ? mt : mt + dy;
    float br = dx > 0 ? mr + dx : mr;
    float bb = dy > 0 ? mb + dy : mb;
    if (br < sl || bl > sr || bb < st || bt > sb) return;
    if (dx == 0 && dy == 0) return;

    // 2. Khoảng cách tới lúc BẮT ĐẦU chạm (entry) và lúc HẾT chạm (exit) trên từng trục
    float dxEntry, dxExit, dyEntry, dyExit;
    if (dx > 0) { dxEntry = sl - mr; dxExit = sr - ml; }
    else { dxEntry = sr - ml; dxExit = sl - mr; }
    if (dy > 0) { dyEntry = st - mb; dyExit = sb - mt; }
    else { dyEntry = sb - mt; dyExit = st - mb; }

    // 3. Đổi khoảng cách thành thời gian (chia cho quãng đi được)
    const float INF = 1e9f;
    float txEntry, txExit, tyEntry, tyExit;
    if (dx == 0) { txEntry = -INF; txExit = INF; }
    else { txEntry = dxEntry / dx; txExit = dxExit / dx; }
    if (dy == 0) { tyEntry = -INF; tyExit = INF; }
    else { tyEntry = dyEntry / dy; tyExit = dyExit / dy; }

    if ((txEntry < 0 && tyEntry < 0) || txEntry > 1 || tyEntry > 1) return;

    // 4. Chạm thật khi CẢ HAI trục đều đang chồng lên nhau
    float tEntry = txEntry > tyEntry ? txEntry : tyEntry;
    float tExit = txExit < tyExit ? txExit : tyExit;
    if (tEntry > tExit) return;

    t = tEntry;
    if (txEntry > tyEntry) { nx = dx > 0 ? -1.0f : 1.0f; }   // trục X chạm sau cùng → đụng mặt bên
    else { ny = dy > 0 ? -1.0f : 1.0f; }   // trục Y chạm sau cùng → đụng mặt trên/dưới
}

void CCollision::Process(CGameObject* src, float dt, std::vector<CGameObject*>* coObjects)
{
    float x, y;
    src->GetPosition(x, y);
    float vx, vy;
    src->GetSpeed(vx, vy);

    float dx = dt * vx;
    float dy = dt * vy;

    if (dx != 0)
    {
        CCollisionEvent hit;
        if (FindFirstHit(src, dx, 0, coObjects, hit)) {
            x += hit.t * dx + hit.nx * BLOCK_PUSH;
            src->SetPosition(x, y);
            src->OnCollisionWith(&hit);
        }
        else {
            x += dx;
            src->SetPosition(x, y);
        }
    }

    if (dy != 0)
    {
        CCollisionEvent hit;
        if (FindFirstHit(src, 0, dy, coObjects, hit)) {
            y += hit.t * dy + hit.ny * BLOCK_PUSH;
            src->SetPosition(x, y);
            src->OnCollisionWith(&hit);
        }
        else {
            y += dy;
            src->SetPosition(x, y);
        }
    }
}

bool CCollision::FindFirstHit(CGameObject* src, float dx, float dy, std::vector<CGameObject*>* coObjects, CCollisionEvent& hit)
{
    float ml, mt, mr, mb;
    src->GetBoundingBox(ml, mt, mr, mb);
    hit.obj = nullptr;
    hit.t = 2.0f;

    for (CGameObject* obj : *coObjects)
    {
        if (obj == src || obj->IsDeleted() || !obj->IsBlock())
            continue;

        float sl, st, sr, sb;
        obj->GetBoundingBox(sl, st, sr, sb);
        float t = 2.0f, nx, ny;
        SweptAABB(ml, mt, mr, mb, dx, dy, sl, st, sr, sb, t, nx, ny);
        if (t < hit.t && t >= 0.0f && t <= 1.0f) 
        {
            hit.obj = obj;
            hit.t = t;
            hit.nx = nx;
            hit.ny = ny;
        }
    }
    return hit.obj != nullptr;
}
