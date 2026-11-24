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

	if (!map.Load(MAP_PATH_LEVEL1, ID_TEX_TILESET))
		DebugOut(L"[ERROR] Khong nap duoc map, xem cac dong [MAP] o tren\n");

	BuildColliders(map.FindLayer("Solid"), false);
	BuildColliders(map.FindLayer("SemiSolid"), true);
	SpawnObjects();

	if (player == nullptr)          // map chưa có object Player
	{
		DebugOut(L"[MAP] Khong co object Player, dung vi tri mac dinh\n");
		player = new CPlayer(PLAYER_START_X, PLAYER_START_Y);
		objects.push_back(player);
	}
	player->SetFallLimit(map.GetHeightPx() + 32.0f);

	CGame* game = CGame::GetInstance();
	camera.SetViewSize(game->GetBackBufferWidth(), game->GetBackBufferHeight());
	camera.SetBounds(0, 0, map.GetWidthPx(), map.GetHeightPx());
	camera.Follow(player);
	camera.SnapToTarget();
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
	map.Render();
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

#define MAX_MERGE_TILES 16
void CPlayScene::BuildColliders(const TileLayer* layer, bool semiSolid)
{
	if (layer == nullptr)
		return;

	int tileW = map.GetTileW();
	int tileH = map.GetTileH();
	int newColliderCount = 0;
	for (int row = 0; row < map.GetHeight(); row++)
	{
		int start = -1;
		for (int col = 0; col <= map.GetWidth(); col++)
		{
			bool filled = map.GetTile(layer, col, row) != 0;
			bool full = (start != -1 && col - start >= MAX_MERGE_TILES);

			if (start != -1 && (!filled || full))
			{
				objects.push_back(new CCollider(start * tileW, row * tileH, (col - start) * tileW, tileH, semiSolid));
				newColliderCount++;
				start = -1;
			}

			if (filled && start == -1)
				start = col;
		}
	}
	DebugOut(L"BuildColliders [%S]: Them %d collider vao objects\n", layer->name.c_str(), newColliderCount);
}
void CPlayScene::SpawnObjects()
{
	for (const MapObject& o : map.GetObjects())
	{
		float cx = o.x + o.width / 2;     
		float cy = o.y + o.height / 2;   
		float bottom = o.y + o.height;        

		if (o.type == "Player")
		{
			if (player != nullptr)
			{
				DebugOut(L"Warning: Ton tai hai class player trong scene -> 'khong tao player thu hai'");
				continue;
			}
			player = new CPlayer(cx, bottom - PlayerStats::FEET_OFFSET);
			objects.push_back(player);
			continue;
		}

		if (o.type == "QuestionBlock" || o.type == "Brick" )
		{
			objects.push_back(new CBrick(cx, cy));
			continue;
		}
		
		DebugOut(L"[MAP] Chua ho tro Class '%S'\n", o.type.c_str());
	}
}

