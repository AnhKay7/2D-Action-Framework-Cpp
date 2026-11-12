#include "Input.h"

CInput* CInput::instance = NULL;

CInput* CInput::GetInstance()
{
    if (instance == NULL)
        instance = new CInput();
    return instance;
}

void CInput::Update()
{
    for (int k = 0; k < 256; k++)
        previous[k] = current[k];

    if (GetForegroundWindow() != hWnd)
    {
        for (int k = 0; k < 256; k++)
            current[k] = false;
        return;
    }
    //current[k] = (GetAsyncKeyState(k) & 0x8000) != 0;
    //(bit 0x8000 = "đang bị đè")
    for (int k = 0; k < 256; k++)
        current[k] = (GetAsyncKeyState(k) & 0x8000) != 0;
}

bool CInput::IsHeld(int key) 
{
    if (key < 0 || key > 255)
        return false;
    return current[key];
}
bool CInput::IsPressed(int key) 
{
    if (key < 0 || key > 255)
        return false;
    return current[key] && !previous[key];
}
bool CInput::IsReleased(int key)
{
    if (key < 0 || key > 255)
        return false;
    return !current[key] && previous[key];
}