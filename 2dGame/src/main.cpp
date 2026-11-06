#include <Windows.h>
#include "Engine/Debug.h"

#pragma comment(lib, "winmm.lib")

#define WINDOW_CLASS_NAME	L"2GameWindow"
#define WINDOW_TITLE		L"2D Game - SE102"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int clientWidth, int clientHeight) 
{
	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = WINDOW_CLASS_NAME;
	if (!RegisterClassExW(&wc))
	{
		DebugOut(L"[ERROR] RegisterClassEx failed\n");
		return NULL;
	}

	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
	RECT rect = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&rect, style, FALSE);

	HWND hWnd = CreateWindowW(WINDOW_CLASS_NAME, WINDOW_TITLE, style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, hInstance, NULL
	);

	if (hWnd == NULL) 
	{
		DebugOut(L"[ERROR] Create Window failed\n");
		return NULL;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return hWnd;
}
int WINAPI WinMain(HINSTANCE hIntance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	MessageBox(NULL, L"Xin chao SE102!", L"2dGame", MB_OK);
	//HWND hWnd = CreateGamgeWindow(hIntance, nCmdShow, SREEN)
	return 0;
}

