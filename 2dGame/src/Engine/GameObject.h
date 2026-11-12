#pragma once

#include <vector>
struct CCollisionEvent;

class CGameObject
{
protected:                 
    float x = 0, y = 0;    
    float vx = 0, vy = 0;  
    int   nx = 1;          
    int   state = -1;
    bool  isDeleted = false;

public:
    CGameObject(float x, float y) { this->x = x; this->y = y; }
    virtual ~CGameObject() {}

    virtual void Update(int dt, std::vector<CGameObject*>* coObjects = nullptr) = 0;
    virtual void Render() = 0;
    virtual void SetState(int state) { this->state = state; }
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;

    virtual int IsBlock() { return 1; }

    void Delete() { isDeleted = true; }
    bool IsDeleted() { return isDeleted; }

    void RenderBoundingBox();

    float GetX() { return x; }
    float GetY() { return y; }
    int   GetState() { return state; }

    virtual void OnCollisionWith(CCollisionEvent* e) {}

    void GetPosition(float& x, float& y) { x = this->x; y = this->y; }   
    void SetPosition(float x, float y) { this->x = x; this->y = y; }     
    void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; } 
};
