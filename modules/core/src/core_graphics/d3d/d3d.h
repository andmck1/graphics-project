#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>
#include <core_window/window.h>
#include <vector>

using namespace Microsoft::WRL;
using namespace DirectX;

struct D3DAppState
{
    Window *window = nullptr;
};

class D3D
{
    public:
    D3DAppState *state;
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> deviceContext;
    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11RenderTargetView> renderTargetView;
    XMMATRIX projectionMatrix;
    XMMATRIX orthographicProjectionMatrix;
    D3D11_VIEWPORT viewport = {};
    ComPtr<ID3D11RasterizerState> rasterizerState;
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    ComPtr<ID3DBlob> errorBlob;
    ComPtr<ID3D11Buffer> matrixBuffer;
    ComPtr<ID3D11Texture2D> depthStencilBuffer;
    ComPtr<ID3D11DepthStencilState> depthStencilStateDepthEnabled;
    ComPtr<ID3D11DepthStencilState> depthStencilStateDepthDisabled;
    ComPtr<ID3D11DepthStencilView> depthStencilView;

    D3D(D3DAppState *state);

    bool init();
    ComPtr<ID3D11VertexShader> createVertexShader(ComPtr<ID3DBlob> shaderBlob);
    ComPtr<ID3D11PixelShader> createPixelShader(ComPtr<ID3DBlob> shaderBlob);
    ComPtr<ID3D11InputLayout> createInputLayout(
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescription,
        ComPtr<ID3DBlob> shaderBlob
    );
    bool createBackBufferRenderTargetView();
    bool setRenderTargetView(
        ComPtr<ID3D11RenderTargetView> _renderTargetView,
        ComPtr<ID3D11DepthStencilView> _depthStencilView
    );
    bool setRenderTargetViewToBackBuffer();
    bool setDefaultViewport();
    bool setViewport(D3D11_VIEWPORT _viewport); 
    bool clearBackBuffer(const float clearColour[4]);
    bool clearRenderTarget(
        const float clearColour[4],
        ComPtr<ID3D11RenderTargetView> _renderTargetView,
        ComPtr<ID3D11DepthStencilView> _depthStencilView
    );
    void setDepthStencilState(bool isEnabled);
    bool presentScreen();

    static ComPtr<ID3DBlob> loadShader(std::string shaderFilepath);

    private:
    bool createDeviceAndSwapChain();
    bool initProjectionMatrix();
    bool setDepthBuffer();
    bool createDepthStencilEnabledState();
    bool createDepthStencilDisabledState();
    bool setStencilBuffer();
    bool setRasterizerState();
    bool createMatrixBuffer();
};
