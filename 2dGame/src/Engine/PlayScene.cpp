#include "PlayScene.h"
#include "Textures.h"
#include "Animations.h"
#include "Sprites.h"
#include "Game.h"
#include "Input.h"

#include "Game/AssetIDs.h"
#include "Game/Player.h"
#include "Game/Brick.h"

#include <algorithm>

void AddStrip(int texId, int firstSpriteId, int count, int aniId, int frameTime, bool loop)
{
	CTexture* tex = CTextures::GetInstance()->Get(texId);
	if (tex == NULL)
		return;
	CAnimation* ani = new CAnimation(loop);

	for (int i = 0; i < count; i++)
	{
		CSprites::GetInstance()->Add(firstSpriteId + i, 144 * i + 19, 40, 98, 64, tex);
		ani->AddFrame(firstSpriteId + i, frameTime);
	}

	CAnimations::GetInstance()->Add(aniId, ani);
}
void CPlayScene::LoadAssets()
{
	CTextures* texs = CTextures::GetInstance();

	texs->Add(ID_TEX_PLAYER_IDLE, TEX_PATH_PLAYER_IDLE);
	texs->Add(ID_TEX_PLAYER_RUN, TEX_PATH_PLAYER_RUN);
	texs->Add(ID_TEX_PLAYER_ATTACK1, TEX_PATH_PLAYER_ATTACK1);

	AddStrip(ID_TEX_PLAYER_IDLE, ID_SPRITE_PLAYER_IDLE, 7, ID_ANI_PLAYER_IDLE, 100, true);
	AddStrip(ID_TEX_PLAYER_RUN, ID_SPRITE_PLAYER_RUN, 8, ID_ANI_PLAYER_RUN, 80, true);
	AddStrip(ID_TEX_PLAYER_ATTACK1, ID_SPRITE_PLAYER_ATTACK1, 10, ID_ANI_PLAYER_ATTACK1, 60, false);

	texs->Add(ID_TEX_BRICK, TEX_PATH_BRICK);
	texs->Add(ID_TEX_BBOX, TEX_PATH_BBOX);

	CSprites* sprites = CSprites::GetInstance();

	sprites->Add(ID_SPRITE_BRICK, 0, 0, 16, 16, texs->Get(ID_TEX_BRICK));
}

void CPlayScene::PurgeDeletedObjects()
{
	for (CGameObject*& obj : objects)
	{
		if (obj->IsDeleted())
		{
			delete obj;
			obj = nullptr;
		}
	}

	objects.erase(std::remove(objects.begin(), objects.end(), nullptr), objects.end());
}

void CPlayScene::Load()
{
	LoadAssets();

	CGame* game = CGame::GetInstance();
	float bbWidth = game->GetBackBufferWidth();
	float bbHeight = game->GetBackBufferHeight();

	player = new CPlayer(bbWidth / 2, bbHeight / 2);
	objects.push_back(player);

	for (int i = 0; i < 20; i++)
		objects.push_back(new CBrick(16 * i + 8, 128 + 8));
}

void CPlayScene::Update(int dt)
{
	CInput* input = CInput::GetInstance();

	if (input->IsPressed(VK_F1))
		showBBox = !showBBox;
	if (input->IsPressed('K'))
	{
		for (auto obj : objects) 
		{
			if (dynamic_cast<CBrick*>(obj) != nullptr)
			{
				obj->Delete();
				break;
			}
		}
	}

	for (CGameObject* obj : objects)
	{
		obj->Update(dt);
	}

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (CGameObject* obj : objects)
	{
		obj->Render();
		if (showBBox)
			obj->RenderBoundingBox();
	}
}

void CPlayScene::Unload()
{
	for (CGameObject* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	player = nullptr;
}
