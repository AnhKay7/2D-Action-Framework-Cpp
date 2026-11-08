#include "Game.h"
#include "Debug.h"
#include <cfloat>   // FLT_MAX
#include <cmath>    // floorf

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

	hr = D3DX10CreateSprite(device, 0, &spriteObject);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] D3DX10CreateSprite failed 0x%08X\n", hr);
		return false;
	}

	D3DXMATRIX matProjection;
	D3DXMatrixOrthoOffCenterLH(&matProjection,
		0.0f, (float)bbWidth,     // trái, phải
		0.0f, (float)bbHeight,    // dưới, trên
		0.1f, 10.0f);             // gần, xa (game 2D không quan tâm lắm)
	spriteObject->SetProjectionTransform(&matProjection);

	D3D10_BLEND_DESC bd = {};
	bd.BlendEnable[0] = TRUE;
	bd.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	bd.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	bd.BlendOp = D3D10_BLEND_OP_ADD;
	bd.SrcBlendAlpha = D3D10_BLEND_ZERO;
	bd.DestBlendAlpha = D3D10_BLEND_ZERO;
	bd.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	bd.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&bd, &blendStateAlpha);

	D3D10_SAMPLER_DESC sd2 = {};
	sd2.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;
	sd2.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
	sd2.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
	sd2.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
	sd2.MaxAnisotropy = 1;
	sd2.ComparisonFunc = D3D10_COMPARISON_NEVER;
	sd2.MinLOD = -FLT_MAX;
	sd2.MaxLOD = FLT_MAX;
	device->CreateSamplerState(&sd2, &pointSampler);

	DebugOut(L"[INFO] Direct3D 10 OK, back buffer %dx%d\n", bbWidth, bbHeight);
	return true;
}

void CGame::BeginRender(float r, float g, float b)
{
	float color[4] = { r, g, b, 1.0f };

	device->ClearRenderTargetView(renderTargetView, color);

	spriteObject->Begin(D3DX10_SPRITE_SORT_TEXTURE);    // cầm cọ lên

	float blendFactor[4] = { 0, 0, 0, 0 };
	device->OMSetBlendState(blendStateAlpha, blendFactor, 0xffffffff);  // bật giấy bóng kính
	device->PSSetSamplers(0, 1, &pointSampler);
}

void CGame::EndRender()
{
	spriteObject->End();
	swapChain->Present(1, 0);
}

CTexture* CGame::LoadTexture(LPCWSTR filePath)
{
	D3DX10_IMAGE_LOAD_INFO info = {};
	info.Width = D3DX10_DEFAULT;          
	info.Height = D3DX10_DEFAULT;
	info.Depth = D3DX10_DEFAULT;
	info.FirstMipLevel = 0;
	info.MipLevels = 1;                   
	info.Usage = D3D10_USAGE_DEFAULT;
	info.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	info.CpuAccessFlags = 0;
	info.MiscFlags = 0;
	info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   
	info.Filter = D3DX10_FILTER_NONE;     
	info.MipFilter = D3DX10_FILTER_NONE;
	info.pSrcInfo = NULL;

	ID3D10Resource* resource = NULL;
	HRESULT hr = D3DX10CreateTextureFromFile(device, filePath, &info, NULL, &resource, NULL);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] Khong nap duoc anh: %s (0x%08X)\n", filePath, hr);
		return NULL;
	}

	ID3D10Texture2D* tex = NULL;
	resource->QueryInterface(__uuidof(ID3D10Texture2D), (void**)&tex);
	resource->Release();
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] %s khong phai anh 2D\n", filePath);
		return NULL;
	}

	ID3D10ShaderResourceView* view = NULL;
	hr = device->CreateShaderResourceView(tex, NULL, &view);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] CreateShaderResourceView failed 0x%08X\n", hr);
		tex->Release();
		return NULL;
	}

	DebugOut(L"[INFO] Nap anh OK: %s\n", filePath);
	return new CTexture(tex, view);
}

void CGame::DrawSprite(float x, float y, int width, int height, D3DX10_SPRITE& sprite) 
{
	D3DXMATRIX matScale, matMove;
	D3DXMatrixScaling(&matScale, (float) width, (float)height, 1.0f);

	D3DXMatrixTranslation(&matMove, floorf(x), backBufferHeight - floorf(y), 0.1f);

	sprite.matWorld = matScale * matMove;  
	spriteObject->DrawSpritesImmediate(&sprite, 1, 0, 0);
}
void CGame::Draw(float x, float y, CTexture* tex)
{
	if (tex == NULL) return;

	D3DX10_SPRITE sprite = {};
	sprite.pTexture = tex->GetView();
	sprite.TexCoord.x = 0.0f;   
	sprite.TexCoord.y = 0.0f;
	sprite.TexSize.x = 1.0f;    
	sprite.TexSize.y = 1.0f;
	sprite.TextureIndex = 0;
	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   

	DrawSprite(x, y, tex->GetWidth(), tex->GetHeight(), sprite);
}
void CGame::DrawRegion(float x, float y, CTexture* tex,
	int left, int top, int width, int height, bool flipX)
{
	if (tex == NULL) return;

	D3DX10_SPRITE sprite = {};
	sprite.pTexture = tex->GetView();
	sprite.TextureIndex = 0;
	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	float texW = (float)tex->GetWidth();
	float texH = (float)tex->GetHeight();

	sprite.TexCoord.x = left / texW;
	sprite.TexCoord.y = top / texH;
	sprite.TexSize.x = width / texW;
	sprite.TexSize.y = height / texH;

	if (flipX) 
	{
		sprite.TexCoord.x = (left + width) / texW;
		sprite.TexSize.x = -sprite.TexSize.x;
	}

	DrawSprite(x, y, width, height, sprite);
}

void CGame::Release()
{
	if (pointSampler)
	{
		pointSampler->Release();
		pointSampler = NULL;
	}
	if (blendStateAlpha)
	{
		blendStateAlpha->Release();
		blendStateAlpha = NULL;
	}
	if (spriteObject)
	{
		spriteObject->Release();
		spriteObject = NULL;
	}
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