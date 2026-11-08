#pragma once
#include <unordered_map>
#include "Sprite.h"

class CSprites
{
    static CSprites* instance;
    std::unordered_map<int, CSprite*> sprites;
    CSprites() {}

public:
    static CSprites* GetInstance();

    void Add(int id, int left, int top, int width, int height, CTexture* tex);
    CSprite* Get(int id);
    void Clear();
};