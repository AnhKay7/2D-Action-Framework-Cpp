#include "QuestionBlock.h"
#include "Game/AssetIDs.h"
#include "Game/PlayScene.h"
#include "Game/Player.h"
#include "Game/CoinPop.h"
#include "Game/GrowItem.h"

CQuestionBlock::CQuestionBlock(float x, float y, BlockContent content)
    : CBlock(x, y), content(content)
{
    SetState(QBLOCK_STATE_ACTIVE);
}

void CQuestionBlock::SetState(int newState)
{
    CGameObject::SetState(newState);
    if (newState == QBLOCK_STATE_ACTIVE)
        animator.Play(ID_ANI_QBLOCK);
    else
        animator.Play(ID_ANI_QBLOCK_EMPTY);
}

void CQuestionBlock::OnHitFromBelow(CGameObject* by)
{
    if (state == QBLOCK_STATE_EMPTY)
        return;

    StartBump();
    SpawnContent(by);
}

void CQuestionBlock::SpawnContent(CGameObject* by)
{

    if (content == BlockContent::Coin)
    {
        CCoinPop* obj = new CCoinPop(x, y - 16);
        CPlayScene::current->Spawn(obj);
        CPlayer* p = dynamic_cast<CPlayer*>(by);
        if (p != nullptr)
            p->AddCoin();
    }
    if (content == BlockContent::Power)
    {
        CGrowItem* obj = new CGrowItem(x, y);
        CPlayScene::current->Spawn(obj, true);
    }

    SetState(QBLOCK_STATE_EMPTY);
}
