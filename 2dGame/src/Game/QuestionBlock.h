#pragma once
#include "Game/Block.h"

enum class BlockContent { Coin, Power };

#define QBLOCK_STATE_ACTIVE 100     // còn đồ bên trong, có animation
#define QBLOCK_STATE_EMPTY  200     // đã đội, thành khối rỗng

class CQuestionBlock : public CBlock
{
    BlockContent content;

    void SpawnContent(CGameObject* by);     // bạn viết

public:
    CQuestionBlock(float x, float y, BlockContent content);

    void SetState(int state) override;
    void OnHitFromBelow(CGameObject* by) override;  // bạn viết
};
