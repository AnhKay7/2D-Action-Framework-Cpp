#pragma once
#include "Texture.h"

class CSprite
{
private:
    int id;
    int left, top, width, height;
    CTexture* texture;

public:
    CSprite(int id, int left, int top, int width, int height, CTexture* tex);
    void Draw(float x, float y, bool flipX = false);
};