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
    ComPtr<ID3D11RenderTargetView> backBufferRenderTargetView;
    XMMATRIX projectionMatrix;
    D3D11_VIEWPORT defaultViewport = {};
    D3D11_DEPTH_STENCIL_VIEW_DESC defaultDepthStencilViewDescription = {};
    ComPtr<ID3D11RasterizerState> rasterizerState;
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    ComPtr<ID3DBlob> errorBlob;
    ComPtr<ID3D11Buffer> matrixBuffer;
    ComPtr<ID3D11Texture2D> depthStencilBuffer;
    ComPtr<ID3D11DepthStencilState> depthStencilState;
    ComPtr<ID3D11DepthStencilView> backBufferDepthStencilView;

    D3D(D3DAppState *state);

    bool init();
    ComPtr<ID3D11VertexShader> createVertexShader(ComPtr<ID3DBlob> shaderBlob);
    ComPtr<ID3D11PixelShader> createPixelShader(ComPtr<ID3DBlob> shaderBlob);
    ComPtr<ID3D11InputLayout> createInputLayout(
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescription,
        ComPtr<ID3DBlob> shaderBlob
    );
    bool clearScreen(const float clearColour[4]);
    bool presentScreen();

    static ComPtr<ID3DBlob> loadShader(std::string shaderFilepath);

    bool setRenderTargetViewToBackBuffer();
    bool setRenderTargetView(
        ComPtr<ID3D11RenderTargetView> renderTargetView,
        ComPtr<ID3D11DepthStencilView> depthStencilView
    );
    bool setViewport(D3D11_VIEWPORT *viewport);

    private:
    bool createDeviceAndSwapChain();
    void initProjectionMatrix();
    bool createBackBufferRenderTargetView();
    bool setDepthBuffer();
    bool createDepthStencilState();
    bool setDepthStencilState();
    void initDefaultDepthStencilDescription();
    bool createBackBufferStencilView();
    bool setRasterizerState();
    void initDefaultViewport();
    bool createMatrixBuffer();
};
