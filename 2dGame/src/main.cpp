#include <Windows.h>
#include "Engine/Debug.h"
#include "Engine/Game.h"

#pragma comment(lib, "winmm.lib")

#define WINDOW_CLASS_NAME	L"2GameWindow"
#define WINDOW_TITLE		L"2D Game - SE102"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BACKBUFFER_WIDTH  320
#define BACKBUFFER_HEIGHT 240

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) {
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
	if (!RegisterClassExW(&wc)){
		DebugOut(L"[ERROR] RegisterClassEx failed\n");
		return NULL;
	}

	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
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
float testX = 0.0f;
float testVx = 0.1f;
void Update(float dt) 
{
	testX += testVx * dt;
	if (testX < 0 || testX > SCREEN_WIDTH) testVx = -testVx;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (hWnd == NULL) 
	{
		return 0;
	}

	SetDebugWindow(hWnd);
	CGame* game = CGame::GetInstance();
	if (!game->Init(hWnd, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT))
		return 0;
	CTexture* texTest = game->LoadTexture(L"assets\\test\\test.png");
	MSG msg = {};
	bool running = true;
	ULONGLONG last = GetTickCount64();

	while (running) 
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();
		float dt = (float)(now - last);
		last = now;
		Update(dt);

		DebugOutTitle(L"x = %.1f", testX);

		game->BeginRender(testX / SCREEN_WIDTH, 0.2f, 0.4f);
		game->Draw(testX / 2, 120, texTest);
		game->EndRender();
	}

	delete texTest;
	game->Release();
	return 0;
}

