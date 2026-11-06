#pragma once
#include <Windows.h>
#include <d3d10.h>

class CGame
{
private:
	static CGame* instance;

	HWND hWnd = NULL;
	int backBufferWidth = 0;
	int backBufferHeight = 0;

	ID3D10Device* device = NULL;
	IDXGISwapChain* swapChain = NULL;
	ID3D10RenderTargetView* renderTargetView = NULL;

	CGame() {}

public:
	static CGame* GetInstance();

	bool Init(HWND hWnd, int bbWidth, int bbHeight);
	void BeginRender(float r, float g, float b);
	void EndRender();
	void Release();

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }
};

