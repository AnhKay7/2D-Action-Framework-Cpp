#pragma once
#include <Windows.h>

void SetDebugWindow(HWND hWnd);
void DebugOut(const wchar_t* fmt, ...);
void DebugOutTitle(const wchar_t* fmt, ...);


