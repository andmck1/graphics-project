#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d2d1.h>
#include <dwrite.h>
#include <directxmath.h>

#include <wrl/client.h>
#include <string>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

struct DeviceManager
{
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGISwapChain> swapChain;

    static DeviceManager &Get();

    bool CreateDeviceAndSwapChain(HWND hwnd, int screenWidth, int screenHeight);
};

struct ShaderManager
{
    ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3D11InputLayout> inputLayout;

    bool CompileShaders();
    bool CreateInputLayout();
};

struct Renderer
{
    ComPtr<ID3D11RenderTargetView> renderTargetView;

    bool CreateRenderTarget();
    bool SetRasteriserState();
    bool SetViewport(int width, int height);
    void ClearScreen(float clearColour[4]);
    void DrawScreen(
        DirectX::XMMATRIX world, DirectX::XMMATRIX view,
        DirectX::XMMATRIX projection
    );
    bool PresentScreen();
    bool RenderFrame(float clearColour[4], std::wstring fpsText);
};

struct TextRenderer
{
    ComPtr<ID2D1Factory> d2dFactory;
    ComPtr<ID2D1RenderTarget> d2dRenderTarget;
    ComPtr<IDWriteFactory> dwriteFactory;
    ComPtr<IDWriteTextFormat> textFormat;
    ComPtr<ID2D1SolidColorBrush> textBrush;

    bool CreateD2D1Factory();
    bool CreateD2DRenderTarget();
    bool CreateDWriteFactory();
    bool CreateTextFormat();
    bool CreateSolidColourBrush();

    bool DrawTextOverlay(const std::wstring &text);
};

class D3DApp
{
    public:
    void sendData();
    bool Init(HWND hwnd, int screenWidth, int screenHeight);

    private:
    HWND hwnd = nullptr;
    DeviceManager deviceManager;
    ShaderManager shaderManager;
    Renderer renderer;
    TextRenderer textRenderer;
};
