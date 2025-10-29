#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>
#include <window/window.h>

#include "light/point.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class D3D
{
    public:
    Window *window;
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> deviceContext;
    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11RenderTargetView> renderTargetView;
    XMMATRIX projectionMatrix;
    D3D11_VIEWPORT viewport = {};
    ComPtr<ID3D11RasterizerState> rasterizerState;
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;
    // TODO - make this a vector of shaders?
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3D11InputLayout> inputLayout;
    ComPtr<ID3D11Buffer> matrixBuffer;
    ComPtr<ID3D11Texture2D> depthStencilBuffer;
    ComPtr<ID3D11DepthStencilState> depthStencilState;
    ComPtr<ID3D11DepthStencilView> depthStencilView;

    D3D(Window *window);

    bool init();
    bool updateMatrixBuffer(XMMATRIX worldMatrix, XMMATRIX viewMatrix);
    bool clearScreen(const float clearColour[4]);
    bool presentScreen();

    private:
    bool createDeviceAndSwapChain();
    bool initProjectionMatrix();
    bool setRenderTargetView();
    bool setDepthBuffer();
    bool setStencilBuffer();
    bool setRasterizerState();
    bool setViewport();
    bool compileShaders();
    bool createShaders();
    bool createInputLayout();
    bool createMatrixBuffer();
};

class Buffer
{
    public:
    virtual bool init(D3D *graphicsManager) = 0;
    virtual bool setPixelShader() = 0;
    ComPtr<ID3D11Buffer> buffer;
    D3D11_BUFFER_DESC bufferDesc;

    protected:
    D3D *graphicsManager;
};

class PointLightBuffer : Buffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setPixelShader() override;

    PointLight *pointLight;
    struct ShaderBuffer
    {
        XMFLOAT4 colour;
        XMFLOAT4 worldPosition;
        XMFLOAT4 direction;
    };

};
