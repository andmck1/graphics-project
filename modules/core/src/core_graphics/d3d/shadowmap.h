#pragma once

#include "core_graphics/buffer/base.h"
#include "d3d.h"

// (Robertson, n.d.)
// (Hughes, n.d.)
// (Falconer, n.d.)
class ShadowMap
{
    public:
    D3D *graphicsManager;
    int width, height;

    void init();
    bool set();
    bool setPixelShader(BufferSlots *slotCounters);

    private:
    ComPtr<ID3D11Texture2D> depthTexture;
    ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    ComPtr<ID3D11DepthStencilView> depthStencilView;
    ComPtr<ID3D11RenderTargetView> nullRTVs[1];
    D3D11_VIEWPORT viewport;
};
