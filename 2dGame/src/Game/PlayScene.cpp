#include "PlayScene.h"
#include "Engine/Textures.h"
#include "Engine/Animations.h"
#include "Engine/Sprites.h"
#include "Engine/Game.h"
#include "Engine/Input.h"
#include "Engine/Debug.h"

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
		CSprites::GetInstance()->Add(firstSpriteId + i, 144 * i + 22, 40, 98, 64, tex);
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
	texs->Add(ID_TEX_PLAYER_JUMP, TEX_PATH_PLAYER_JUMP);
	texs->Add(ID_TEX_PLAYER_FALL, TEX_PATH_PLAYER_FALL);

	AddStrip(ID_TEX_PLAYER_IDLE, ID_SPRITE_PLAYER_IDLE, 7, ID_ANI_PLAYER_IDLE, 100, true);
	AddStrip(ID_TEX_PLAYER_RUN, ID_SPRITE_PLAYER_RUN, 8, ID_ANI_PLAYER_RUN, 80, true);
	AddStrip(ID_TEX_PLAYER_ATTACK1, ID_SPRITE_PLAYER_ATTACK1, 10, ID_ANI_PLAYER_ATTACK1, 60, false);
	AddStrip(ID_TEX_PLAYER_JUMP, ID_SPRITE_PLAYER_JUMP, 4, ID_ANI_PLAYER_JUMP, 80, false);
	AddStrip(ID_TEX_PLAYER_FALL, ID_SPRITE_PLAYER_FALL, 4, ID_ANI_PLAYER_FALL, 80, true);


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
			if (obj == player) 
			{
				player = nullptr;
				camera.Follow(nullptr);
			}
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

	for (int i = 0; (16 * i + 8) < MAP_RIGHT; i++)
	{
		if (i >= 30 && i <= 32)
			continue;
		objects.push_back(new CBrick(16 * i + 8, 128 + 8));
	}

	for (int i = 0; i < 4; i++)
		objects.push_back(new CBrick(MAP_LEFT + BRICK_SIZE / 2, 128 - 16 * i));

	for (int i = 0; i < 4; i++)
		objects.push_back(new CBrick(MAP_RIGHT - BRICK_SIZE / 2, 128 - 16 * i));

	for (int i = 0; i < 3; i++)
		objects.push_back(new CBrick(bbWidth / 3, 128 - 16 * i));

	for (int i = 0; i < 3; i++)
		objects.push_back(new CBrick(216 + 16 * i, 80));

	camera.SetViewSize(bbWidth, bbHeight);
	camera.SetBounds(MAP_LEFT, MAP_TOP, MAP_RIGHT, MAP_BOTTOM);
	camera.Follow(player);
	camera.SnapToTarget();
	//game->SetCamPos(40, 0);
}

void CPlayScene::Update(float dt)
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
		obj->Update(dt, &objects);
	}

	PurgeDeletedObjects();

	camera.Update(dt);

	if (player != nullptr)
		DebugOutTitle(L"onGround=%d  vy=%.3f  touchingWall=%d  wallDir=%d",
			player->IsOnGround(), player->GetVy(), player->IsTouchingWall(), player->GetWallDirection());
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
	camera.Follow(nullptr);
}
