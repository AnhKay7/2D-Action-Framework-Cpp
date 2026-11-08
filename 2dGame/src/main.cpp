#include <Windows.h>
#include "Engine/Debug.h"
#include "Engine/Game.h"
#include "Engine/Textures.h"
#include "Engine/Sprites.h"
#include "Engine/Animations.h"
#include "Engine/Animator.h"
#include "Engine/Input.h"

#pragma comment(lib, "winmm.lib")

#define WINDOW_CLASS_NAME	L"2GameWindow"
#define WINDOW_TITLE		L"2D Game - SE102"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BACKBUFFER_WIDTH  320
#define BACKBUFFER_HEIGHT 240

#define ID_TEX_IDLE    10
#define ID_TEX_RUN     11
#define ID_TEX_ATTACK1 12

#define ID_SPRITE_IDLE    1000   // 1000..1006
#define ID_SPRITE_RUN     1100   // 1100..1107
#define ID_SPRITE_ATTACK1 1200   // 1200..1209

#define ID_ANI_IDLE    1
#define ID_ANI_RUN     2
#define ID_ANI_ATTACK1 3

#define PATH_TEX_PLAYER_IDLE L"assets\\player\\Idle.png"
#define PATH_TEX_PLAYER_RUN L"assets\\player\\Run.png"
#define PATH_TEX_PLAYER_ATTACK1 L"assets\\player\\Attack1.png"

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

float testX = 0.0f;
float testVx = 0.1f;

CAnimator playerAnim;
float playerX = 160, playerY = 120;
bool  facingLeft = false;
bool  attacking = false;

void Update(float dt)
{
	//if (CInput::GetInstance()->IsPressed('J'))  DebugOut(L"J pressed\n");
	//if (CInput::GetInstance()->IsReleased('J')) DebugOut(L"J released\n");
	CGame* game = CGame::GetInstance();
	CInput* input = CInput::GetInstance();

	testX += testVx * dt;
	if (testX < 0 || testX > SCREEN_WIDTH)
		testVx = -testVx;

	int moveDirection = 0;

	if (input->IsHeld(VK_RIGHT) || input->IsHeld('D'))
		moveDirection += 1;
	if (input->IsHeld(VK_LEFT) || input->IsHeld('A'))
		moveDirection -= 1;
	if (!attacking && input->IsPressed('X')) {
		attacking = true;
		playerAnim.Play(ID_ANI_ATTACK1, true);
	}

	if (!attacking) {
		playerX += moveDirection * dt * 0.08f;

		if (moveDirection > 0)
			facingLeft = false;
		if (moveDirection < 0)
			facingLeft = true;
	}

	if (playerX + 144 / 2 > game->GetBackBufferWidth())
		playerX = game->GetBackBufferWidth() - 144 / 2;
	if (playerX < 144 / 2)
		playerX = 144 / 2;

	if (attacking) {
		if (playerAnim.IsFinished())
			attacking = false;
	}
	else if (moveDirection != 0)
		playerAnim.Play(ID_ANI_RUN);
	else
		playerAnim.Play(ID_ANI_IDLE);

	playerAnim.Update(dt);
}

void AddStrip(int texId, int firstSpriteId, int count, int aniId, int frameTime, bool loop)
{
	CTexture* tex = CTextures::GetInstance()->Get(texId);
	if (tex == NULL)
		return;
	CAnimation* ani = new CAnimation(loop);

	for (int i = 0; i < count; i++)
	{
		CSprites::GetInstance()->Add(firstSpriteId + i, 144 * i + 19, 40, 98, 64, tex);
		ani->AddFrame(firstSpriteId + i, frameTime);
	}

	CAnimations::GetInstance()->Add(aniId, ani);
}
void LoadResources()
{
	CTextures* texs = CTextures::GetInstance();

	texs->Add(ID_TEX_IDLE, PATH_TEX_PLAYER_IDLE);
	texs->Add(ID_TEX_RUN, PATH_TEX_PLAYER_RUN);
	texs->Add(ID_TEX_ATTACK1, PATH_TEX_PLAYER_ATTACK1);

	AddStrip(ID_TEX_IDLE, ID_SPRITE_IDLE, 7, ID_ANI_IDLE, 100, true);
	AddStrip(ID_TEX_RUN, ID_SPRITE_RUN, 8, ID_ANI_RUN, 80, true);
	AddStrip(ID_TEX_ATTACK1, ID_SPRITE_ATTACK1, 10, ID_ANI_ATTACK1, 60, false);

	playerAnim.Play(ID_ANI_IDLE, true);
}
void Render()
{
	CGame* game = CGame::GetInstance();
	playerAnim.Render(playerX, playerY, facingLeft);
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
	LoadResources();
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

		CInput::GetInstance()->Update();

		ULONGLONG now = GetTickCount64();
		float dt = (float)(now - last);
		last = now;
		Update(dt);

		DebugOutTitle(L"x = %.1f", testX);

		game->BeginRender(testX / SCREEN_WIDTH, 0.2f, 0.4f);
		Render();
		game->EndRender();
	}
	CAnimations::GetInstance()->Clear();
	CSprites::GetInstance()->Clear();
	CTextures::GetInstance()->Clear();
	game->Release();
	return 0;
}

