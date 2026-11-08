#include "Debug.h"
#include <cstdarg>
#include <cstdio>

static HWND g_debugWindow = NULL;

void SetDebugWindow(HWND hWnd) 
{
	g_debugWindow = hWnd;
}

void DebugOut(const wchar_t* fmt, ...) 
{
	wchar_t buffer[1024];
	va_list args;
	va_start(args, fmt);
	vswprintf_s(buffer, fmt, args);
	va_end(args);
	OutputDebugStringW(buffer);
}

void DebugOutTitle(const wchar_t* fmt, ...)
{
	if (g_debugWindow == NULL) return;
	wchar_t buffer[256];
	va_list args;
	va_start(args, fmt);
	vswprintf_s(buffer, fmt, args);
	va_end(args);
	SetWindowTextW(g_debugWindow, buffer);
}