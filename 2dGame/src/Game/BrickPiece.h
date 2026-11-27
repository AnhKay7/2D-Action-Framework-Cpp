#pragma once
#include "Engine/GameObject.h"
#include "Engine/Animator.h"
#include "Game/AssetIDs.h"

constexpr float PIECE_VX        = 0.06f;
constexpr float PIECE_VY_HIGH   = -0.35f;
constexpr float PIECE_VY_LOW    = -0.25f;
constexpr float PIECE_GRAVITY   = 0.0012f;
constexpr float PIECE_LIFETIME  = 1500.0f;  // ms, rơi khỏi màn hình thì tự xoá

// Mảnh gạch vỡ: bay lên rồi rơi, KHÔNG va chạm gì (copy, không cần sửa)
class CBrickPiece : public CGameObject
{
    CAnimator animator;
    float life = PIECE_LIFETIME;

public:
    CBrickPiece(float x, float y, int dir, bool high) : CGameObject(x, y)
    {
        vx = dir * PIECE_VX;
        vy = high ? PIECE_VY_HIGH : PIECE_VY_LOW;
        nx = dir;
        animator.Play(ID_ANI_BRICK_PIECE);
    }

    int IsBlock() override { return 0; }

    void Update(float dt, std::vector<CGameObject*>* coObjects = nullptr) override
    {
        vy += PIECE_GRAVITY * dt;
        x += vx * dt;
        y += vy * dt;
        life -= dt;
        if (life <= 0)
            Delete();
        animator.Update(dt);
    }

    void Render() override { animator.Render(x, y, nx < 0); }
    void GetBoundingBox(float& l, float& t, float& r, float& b) override { l = r = x; t = b = y; }
};
