#include "Game.h"
#include "Debug.h"

#pragma comment(lib, "d3d10.lib")

CGame* CGame::instance = NULL;

CGame* CGame::GetInstance()
{
	if (instance == NULL)
		instance = new CGame();
	return instance;
}

bool CGame::Init(HWND hWnd, int bbWidth, int bbHeight)
{
	this->hWnd = hWnd;

	backBufferHeight = bbHeight;
	backBufferWidth = bbWidth;

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = bbWidth;
	sd.BufferDesc.Height = bbHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;

	HRESULT hr = D3D10CreateDeviceAndSwapChain(
		NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &sd, &swapChain, &device
	);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] D3D10CreateDeviceAndSwapChain failed 0x%08X\n", hr);
		return false;
	}

	ID3D10Texture2D* backBuffer = NULL;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**)&backBuffer);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] GetBuffer failed 0x%08X\n", hr);
		return false;
	}

	hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	backBuffer->Release();
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] CreateRenderTargetView failed 0x%08X\n", hr);
		return false;
	}
	device->OMSetRenderTargets(1, &renderTargetView, NULL);

	D3D10_VIEWPORT vp = {};
	vp.Width = bbWidth;
	vp.Height = bbHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	device->RSSetViewports(1, &vp);

	DebugOut(L"[INFO] Direct3D 10 OK, back buffer %dx%d\n", bbWidth, bbHeight);
	return true;
}

void CGame::BeginRender(float r, float g, float b)
{
	float color[4] = { r, g, b, 1.0f };

	device->ClearRenderTargetView(renderTargetView, color);
}

void CGame::EndRender()
{
	swapChain->Present(1, 0);
}

void CGame::Release()
{
	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = NULL;
	}
	if (swapChain)
	{
		swapChain->Release();
		swapChain = NULL;
	}
	if (device)
	{ 
		device->Release();           
		device = NULL; 
	}
}