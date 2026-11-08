#pragma once
#include <unordered_map>
#include "Engine/Texture.h"

class CTextures
{
private:
    static CTextures* instance;
    std::unordered_map<int, CTexture*> textures;   
    CTextures() {}

public:
    static CTextures* GetInstance();

    void Add(int id, LPCWSTR filePath);   
    CTexture* Get(int id);               
    void Clear();                   
};