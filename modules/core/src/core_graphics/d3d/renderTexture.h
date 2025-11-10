#pragma once

#include "core_graphics/buffer/base.h"
#include "d3d.h"

// Some lab code here
class RenderTexture
{
    public:
    D3D *graphicsManager;
    int textureWidth, textureHeight;
    float screenNear, screenFar;

    XMMATRIX projectionMatrix;
    XMMATRIX orthographicProjectionMatrix;

    void init();
    void clear(const float clearColour[4]);
    bool set();
    bool setPixelShader(BufferSlots *slotCounters);

    private:
    ComPtr<ID3D11Texture2D> renderTargetTexture;
    ComPtr<ID3D11RenderTargetView> renderTargetView;
    ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    ComPtr<ID3D11Texture2D> depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> depthStencilView;
    D3D11_VIEWPORT viewport;
};
