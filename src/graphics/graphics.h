#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d2d1.h>
#include <dwrite.h>
#include <directxmath.h>

#include <wrl/client.h>
#include <string>

#include "../vertices/vertices.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class DeviceManager
{
  public:
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGISwapChain> swapChain;

    static DeviceManager &Get();

    bool CreateDeviceAndSwapChain(int width, int height);

  private:
    DeviceManager() = default;
    DeviceManager(DeviceManager &) = delete;
    void operator=(DeviceManager &) = delete;
};

class ShaderManager
{
  public:
    ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3D11InputLayout> inputLayout;

    static ShaderManager &Get();

    bool CompileShaders();
    bool CreateInputLayout();

  private:
    ShaderManager() = default;
    ShaderManager(ShaderManager &) = delete;
    void operator=(ShaderManager &) = delete;
};

class BufferManager
{
  public:
    XMMATRIX projectionMatrix;
    ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> matrixBuffer;

    static BufferManager &Get();

    bool InitMatrices(int width, int height);
    bool CreateVertexBuffer();
    bool CreateMatrixBuffer();
    bool UpdateMatrixBuffer(
        DirectX::XMMATRIX world, DirectX::XMMATRIX view,
        DirectX::XMMATRIX projection
    );

  private:
    BufferManager() = default;
    BufferManager(BufferManager &) = delete;
    void operator=(BufferManager &) = delete;
};

class Renderer
{
  public:
    ComPtr<ID3D11RenderTargetView> renderTargetView;

    static Renderer &Get();

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

  private:
    Renderer() = default;
    Renderer(Renderer &) = delete;
    void operator=(Renderer &) = delete;
};

class TextRenderer
{
  public:
    ComPtr<ID2D1Factory> d2dFactory;
    ComPtr<ID2D1RenderTarget> d2dRenderTarget;
    ComPtr<IDWriteFactory> dwriteFactory;
    ComPtr<IDWriteTextFormat> textFormat;
    ComPtr<ID2D1SolidColorBrush> textBrush;

    static TextRenderer &Get();

    bool CreateD2D1Factory();
    bool CreateD2DRenderTarget();
    bool CreateDWriteFactory();
    bool CreateTextFormat();
    bool CreateSolidColourBrush();

    bool DrawTextOverlay(const std::wstring &text);

  private:
    TextRenderer() = default;
    TextRenderer(TextRenderer &) = delete;
    void operator=(TextRenderer &) = delete;
};

bool InitD3D(int width, int height);
bool InitD2DResources();
#endif
