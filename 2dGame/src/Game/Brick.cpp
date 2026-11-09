#include "Brick.h"
#include "Engine/Sprites.h"
#include "AssetIDs.h"

void CBrick::Render()
{
	CSprite* sprite = CSprites::GetInstance()->Get(ID_SPRITE_BRICK);
	if (sprite == NULL)
		return;

	sprite->Draw(x, y);
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_SIZE / 2;
	r = x + BRICK_SIZE / 2;

	t = y - BRICK_SIZE / 2;
	b = y + BRICK_SIZE / 2;
}
