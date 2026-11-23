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
#include "Game/Collider.h"

#include <algorithm>

void AddStrip(int texId, int firstSpriteId, int firstFrame, int count,
	int frameW, int frameH, int aniId, int frameTime, bool loop)
{
	CTexture* tex = CTextures::GetInstance()->Get(texId);
	if (tex == NULL)
		return;
	CAnimation* ani = new CAnimation(loop);

	for (int i = 0; i < count; i++)
	{
		int cell = firstFrame + i;
		CSprites::GetInstance()->Add(firstSpriteId + cell, frameW * cell, 0, frameW, frameH, tex);
		ani->AddFrame(firstSpriteId + cell, frameTime);
	}

	CAnimations::GetInstance()->Add(aniId, ani);
}

void CPlayScene::LoadAssets()
{
	CTextures* texs = CTextures::GetInstance();

	texs->Add(ID_TEX_PLAYER_SMALL, TEX_PATH_PLAYER_SMALL);
	AddStrip(ID_TEX_PLAYER_SMALL, ID_SPRITE_PLAYER_SMALL, 0, 1, 16, 16, ID_ANI_PLAYER_IDLE, 100, true);
	AddStrip(ID_TEX_PLAYER_SMALL, ID_SPRITE_PLAYER_SMALL, 1, 2, 16, 16, ID_ANI_PLAYER_WALK, 120, true);
	AddStrip(ID_TEX_PLAYER_SMALL, ID_SPRITE_PLAYER_SMALL, 3, 2, 16, 16, ID_ANI_PLAYER_RUN, 60, true);
	AddStrip(ID_TEX_PLAYER_SMALL, ID_SPRITE_PLAYER_SMALL, 5, 1, 16, 16, ID_ANI_PLAYER_SKID, 100, true);
	AddStrip(ID_TEX_PLAYER_SMALL, ID_SPRITE_PLAYER_SMALL, 6, 1, 16, 16, ID_ANI_PLAYER_JUMP, 100, true);
	AddStrip(ID_TEX_PLAYER_SMALL, ID_SPRITE_PLAYER_SMALL, 7, 1, 16, 16, ID_ANI_PLAYER_FALL, 100, true);

	texs->Add(ID_TEX_BRICK, TEX_PATH_BRICK);
	texs->Add(ID_TEX_BBOX, TEX_PATH_BBOX);

	CSprites::GetInstance()->Add(ID_SPRITE_BRICK, 0, 0, 16, 16, texs->Get(ID_TEX_BRICK));
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

	player = new CPlayer(PLAYER_START_X, PLAYER_START_Y);
	objects.push_back(player);

	for (int i = 0; i < 100; i++)
	{
		bool hole = (i >= 20 && i <= 22)      
			|| (i >= 40 && i <= 46)      
			|| (i >= 70 && i <= 79);     
		if (hole)
			continue;
		objects.push_back(new CBrick(16 * i + 8, 136));
	}

	for (int i = 8; i <= 12; i++)
		objects.push_back(new CBrick(16 * i + 8, 72));

	for (int i = 0; i < 6; i++)
	{
		objects.push_back(new CBrick(MAP_LEFT + 8, 120 - 16 * i));
		objects.push_back(new CBrick(MAP_RIGHT - 8, 120 - 16 * i));
	}

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

	for (CGameObject* obj : objects)
	{
		obj->Update(dt, &objects);
	}

	PurgeDeletedObjects();

	camera.Update(dt);

	if (player != nullptr)
		DebugOutTitle(L"state=%d  vx=%.3f  vy=%.3f  meter=%.2f  ground=%d",
			player->GetState(), player->GetVx(), player->GetVy(), player->GetMeter(), player->IsOnGround());
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
