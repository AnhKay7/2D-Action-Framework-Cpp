#pragma once
#include <Windows.h>

class CInput
{
    static CInput* instance;
    bool current[256] = {};    
    bool previous[256] = {};   
    HWND hWnd = NULL;
    CInput() {}

public:
    static CInput* GetInstance();

    void Init(HWND hWnd) { this->hWnd = hWnd; }
    void Update();                 

    bool IsHeld(int key);
    bool IsPressed(int key);
    bool IsReleased(int key);
};