#pragma once
#include <vector>
#include "Engine/Scene.h"
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "Engine/TileMap.h"

class CPlayer;

class CPlayScene : public CScene
{
private:
    std::vector<CGameObject*> objects;
    CPlayer* player = nullptr;
    bool showBBox = false;
    std::vector<CGameObject*> pendingFront, pendingBack;

    void LoadAssets();                  
    void PurgeDeletedObjects();        
    
    CCamera camera;

    CTileMap map;

    void BuildColliders(const TileLayer* layer, bool semiSolid);
    void SpawnObjects();

    void FlushSpawned();
public:
    void Load() override;
    void Update(float dt) override;
    void Render() override;
    void Unload() override;

    static CPlayScene* current;                          
    void Spawn(CGameObject* obj, bool behind = false);   
};