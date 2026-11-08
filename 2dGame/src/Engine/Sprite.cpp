#include "Sprite.h"
#include "Game.h"

CSprite::CSprite(int id, int left, int top, int width, int height, CTexture* tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	texture = tex;
}

void CSprite::Draw(float x, float y, bool flipX)
{
	CGame::GetInstance()->DrawRegion(x, y, texture, left, top, width, height, flipX);
}
