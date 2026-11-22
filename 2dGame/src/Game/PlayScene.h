#pragma once
#include <vector>
#include "Engine/Scene.h"
#include "Engine/GameObject.h"
#include "Engine/Camera.h"

#define MAP_LEFT     0
#define MAP_RIGHT    1600
#define MAP_TOP     -240     // trời phía trên màn hình đầu, để leo bục
#define MAP_BOTTOM   240

class CPlayer;

class CPlayScene : public CScene
{
    std::vector<CGameObject*> objects;
    CPlayer* player = nullptr;
    bool showBBox = false;

    void LoadAssets();                  
    void PurgeDeletedObjects();        
    
    CCamera camera;
public:
    void Load() override;
    void Update(float dt) override;
    void Render() override;
    void Unload() override;
};