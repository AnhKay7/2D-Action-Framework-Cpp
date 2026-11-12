#include <Windows.h>
#include "Engine/Debug.h"
#include "Engine/Game.h"
#include "Engine/Textures.h"
#include "Engine/Sprites.h"
#include "Engine/Animations.h"
#include "Engine/Input.h"

#include "Game/PlayScene.h"


#pragma comment(lib, "winmm.lib")

#define WINDOW_CLASS_NAME	L"2GameWindow"
#define WINDOW_TITLE		L"2D Game - SE102"

#define SCREEN_WIDTH		640
#define SCREEN_HEIGHT		480
#define BACKBUFFER_WIDTH	320
#define BACKBUFFER_HEIGHT	240

#define UPDATES_PER_SECOND 60
#define FIXED_DT           (1000.0f / UPDATES_PER_SECOND)

CScene* scene = nullptr;

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
	if (!RegisterClassExW(&wc)) {
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
	CInput::GetInstance()->Init(hWnd);
	scene = new CPlayScene();
	scene->Load();
	MSG msg = {};
	bool running = true;

	LARGE_INTEGER freq, prev, now;
	QueryPerformanceFrequency(&freq);  // đồng hồ chính xác cỡ micro giây
	QueryPerformanceCounter(&prev);
	const double STEP_MS = 1000.0 / UPDATES_PER_SECOND;   // 16.67 ms thật
	double accumulator = 0;


	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		QueryPerformanceCounter(&now);
		double elapsed = (now.QuadPart - prev.QuadPart) * 1000.0 / freq.QuadPart;
		prev = now;
		if (elapsed > 250) elapsed = 250;   // kéo cửa sổ / dừng debugger: đừng chạy bù cả trăm lần

		accumulator += elapsed;
		while (accumulator >= STEP_MS)
		{
			CInput::GetInstance()->Update();
			scene->Update(FIXED_DT);
			accumulator -= STEP_MS;
		}

		game->BeginRender(0.4f, 0.2f, 0.4f);
		scene->Render();
		game->EndRender();
	}

	scene->Unload();
	delete scene;

	CAnimations::GetInstance()->Clear();
	CSprites::GetInstance()->Clear();
	CTextures::GetInstance()->Clear();
	game->Release();
	return 0;
}

