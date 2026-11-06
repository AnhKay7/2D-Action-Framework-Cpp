#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include "Texture.h"

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

	ID3DX10Sprite* spriteObject = NULL;
	ID3D10BlendState* blendStateAlpha = NULL;
	ID3D10SamplerState* pointSampler = NULL;

	CGame() {}

public:
	static CGame* GetInstance();

	bool Init(HWND hWnd, int bbWidth, int bbHeight);
	void BeginRender(float r, float g, float b);
	void EndRender();
	void Release();

	CTexture* LoadTexture(LPCWSTR filePath);
	void Draw(float x, float y, CTexture* tex);

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }
};

