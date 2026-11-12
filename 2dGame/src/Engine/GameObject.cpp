#include "GameObject.h"
#include "Game.h"
#include "Textures.h"
#include "Game/AssetIDs.h"

void CGameObject::RenderBoundingBox()
{
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	int width = (int)(r - l);
	int height = (int)(b - t);

	float cx = (l + width / 2);
	float cy = (t + height / 2);

	CTexture* bbTex = CTextures::GetInstance()->Get(ID_TEX_BBOX);
	CGame::GetInstance()->DrawRegion(cx, cy, bbTex, 0, 0, width, height);
}
