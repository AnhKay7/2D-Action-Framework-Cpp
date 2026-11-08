#include "Sprites.h"
#include "Game.h"
#include "Debug.h"

CSprites* CSprites::instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (instance == NULL)
		instance = new CSprites();
	return instance;
}

void CSprites::Add(int id, int left, int top, int width, int height, CTexture* tex)
{
	CSprite* newSprite = new CSprite(id, left, top, width, height, tex);
	
	if (sprites.count(id))
		delete sprites[id];

	sprites[id] = newSprite;
}

CSprite* CSprites::Get(int id)
{
	auto it = sprites.find(id);
	if (it == sprites.end()) 
	{
		DebugOut(L"[ERROR] Sprite id %d khong ton tai\n", id);
		return NULL;
	}
	return it->second;
}

void CSprites::Clear()
{
	for (auto& sprite : sprites)
	{
		delete sprite.second;
	}
	sprites.clear();
}
