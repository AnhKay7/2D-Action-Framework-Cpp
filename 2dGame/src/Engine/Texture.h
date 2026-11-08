#pragma once
#include <d3d10.h>
class CTexture
{
private:
    ID3D10Texture2D* tex = NULL;            
    ID3D10ShaderResourceView* view = NULL;
    int width = 0;
    int height = 0;
public:
    CTexture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* view)
    {
        this->tex = tex;
        this->view = view;

        D3D10_TEXTURE2D_DESC desc;
        tex->GetDesc(&desc);
        width = desc.Width;
        height = desc.Height;
    }

    ~CTexture()
    {
        if (view) 
            view->Release();
        if (tex)  
            tex->Release();
    }

    ID3D10ShaderResourceView* GetView() { return view; }
    int GetWidth() { return width; }
    int GetHeight() { return height; }
};

