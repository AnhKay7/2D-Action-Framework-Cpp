#pragma once
#include "Game/Block.h"

// Gạch vỡ được: player nhỏ đội thì nảy, player to đội thì vỡ thành 4 mảnh.
class CBreakableBrick : public CBlock
{
    void Break();                                   // bạn viết

public:
    CBreakableBrick(float x, float y);
    void OnHitFromBelow(CGameObject* by) override;  // bạn viết
};
