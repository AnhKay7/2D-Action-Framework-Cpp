#include "Textures.h"
#include "Game.h"
#include "Debug.h"

CTextures* CTextures::instance = NULL;

CTextures* CTextures::GetInstance()
{
    if (instance == NULL)
        instance = new CTextures();
    return instance;
}

void CTextures::Add(int id, LPCWSTR filePath)
{
    CTexture* newTex = CGame::GetInstance()->LoadTexture(filePath);
    if (newTex == NULL) return;

    if (textures.count(id))
        delete textures[id];
    textures[id] = newTex;
}

CTexture* CTextures::Get(int id)
{
    auto it = textures.find(id);
    if (it == textures.end())
    {
        DebugOut(L"[ERROR] Texture id %d khong ton tai\n", id);
        return NULL;
    }
    return it->second;
}

void CTextures::Clear()
{
    for (auto& tex: textures) 
    {
        delete tex.second;
    }
    textures.clear();
}