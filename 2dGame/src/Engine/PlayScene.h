#pragma once
#include <vector>
#include "Engine/Scene.h"
#include "Engine/GameObject.h"

class CPlayer;

class CPlayScene : public CScene
{
    std::vector<CGameObject*> objects;
    CPlayer* player = nullptr;          // giữ riêng để sau này camera đi theo
    bool showBBox = false;

    void LoadAssets();                  // texture, sprite, animation
    void PurgeDeletedObjects();         // dọn các object đã Delete()

public:
    void Load() override;
    void Update(int dt) override;
    void Render() override;
    void Unload() override;
};