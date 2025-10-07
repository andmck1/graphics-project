#include "graphics/system/d3d.h"
#include "graphics/system/utils.h"

bool DeviceManager::CreateDeviceAndSwapChain(
    HWND hwnd, int screenWidth, int screenHeight
)
{
    HRESULT createDeviceAndSwapChainHR;
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = screenWidth;
    scd.BufferDesc.Height = screenHeight;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0};

    createDeviceAndSwapChainHR = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevels, 1, D3D11_SDK_VERSION, &scd, swapChain.GetAddressOf(),
        device.GetAddressOf(), &featureLevel, context.GetAddressOf()
    );

    return TryDXCall(
        createDeviceAndSwapChainHR, "Failed to create device and swap chain.\n"
    );
}

bool Renderer::CreateRenderTarget()
{
    HRESULT getBufferHR;
    HRESULT createRenderTargetViewHR;
    ComPtr<ID3D11Texture2D> backBuffer;

    getBufferHR = deviceManager.swapChain->GetBuffer(
        0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void **>(backBuffer.GetAddressOf())
    );
    if (!TryDXCall(getBufferHR, "Failed to get back buffer from swap chain.\n"))
    {
        return false;
    }

    createRenderTargetViewHR = deviceManager.device->CreateRenderTargetView(
        backBuffer.Get(), nullptr, renderTargetView.GetAddressOf()
    );

    deviceManager.context->OMSetRenderTargets(
        1, renderTargetView.GetAddressOf(), nullptr
    );
    return true;
}

bool Renderer::SetRasteriserState()
{
    HRESULT createRasteriserHR;
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    ComPtr<ID3D11RasterizerState> rasterizerState;
    DeviceManager &deviceManager = DeviceManager::Get();

    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = FALSE;

    createRasteriserHR = deviceManager.device->CreateRasterizerState(
        &rasterizerDesc, rasterizerState.GetAddressOf()
    );
    if (!TryDXCall(createRasteriserHR, "Failed to create rasteriser.\n"))
    {
        return false;
    }
    deviceManager.context->RSSetState(rasterizerState.Get());
    return true;
}

bool Renderer::SetViewport(int width, int height)
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(width);
    viewport.Height = static_cast<FLOAT>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    DeviceManager::Get().context->RSSetViewports(1, &viewport);
    return true;
}

bool ShaderManager::CompileShaders()
{
    HRESULT compileVertexShaderHR;
    HRESULT compilePixelShaderHR;
    HRESULT createVertexShaderHR;
    HRESULT createPixelShaderHR;
    DeviceManager &deviceManager = DeviceManager::Get();

    compileVertexShaderHR = D3DCompileFromFile(
        L"src\\shaders\\vertex.hlsl", nullptr, nullptr, "main", "vs_4_0", 0, 0,
        vsBlob.GetAddressOf(), errorBlob.GetAddressOf()
    );
    if (!TryDXCall(compileVertexShaderHR, "Failed to compile vertex shader.\n"))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char *) errorBlob->GetBufferPointer());
        }
        return false;
    }

    compilePixelShaderHR = D3DCompileFromFile(
        L"src\\shaders\\pixel.hlsl", nullptr, nullptr, "main", "ps_4_0", 0, 0,
        psBlob.GetAddressOf(), errorBlob.GetAddressOf()
    );
    if (!TryDXCall(compilePixelShaderHR, "Failed to compile pixel shader.\n"))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char *) errorBlob->GetBufferPointer());
        }
        return false;
    }

    createVertexShaderHR = deviceManager.device->CreateVertexShader(
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr,
        vertexShader.GetAddressOf()
    );
    if (!TryDXCall(createVertexShaderHR, "Failed to create vertex shader.\n"))
    {
        return false;
    }

    createPixelShaderHR = deviceManager.device->CreatePixelShader(
        psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr,
        pixelShader.GetAddressOf()
    );
    if (!TryDXCall(createPixelShaderHR, "Failed to create pixel shader.\n"))
    {
        return false;
    }
    return true;
}

bool ShaderManager::CreateInputLayout()
{
    HRESULT createInputLayoutHR;
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    DeviceManager &deviceManager = DeviceManager::Get();

    createInputLayoutHR = deviceManager.device->CreateInputLayout(
        layout, 1, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        inputLayout.GetAddressOf()
    );
    if (!TryDXCall(createInputLayoutHR, "Failed to create input layout.\n"))
    {
        return false;
    }
    return true;
}

bool TextRenderer::CreateD2D1Factory()
{
    HRESULT createFactoryHR;
    createFactoryHR = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactory.GetAddressOf()
    );
    return TryDXCall(createFactoryHR, "Failed to create D2D1 factory.\n");
}

bool TextRenderer::CreateD2DRenderTarget()
{
    HRESULT createDxgiSurfaceRenderTarget;
    HRESULT getBufferHR;
    ComPtr<IDXGISurface> dxgiBackBuffer;
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(
            DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED
        )
    );

    getBufferHR = DeviceManager::Get().swapChain->GetBuffer(
        0, __uuidof(IDXGISurface),
        reinterpret_cast<void **>(dxgiBackBuffer.GetAddressOf())
    );
    if (!TryDXCall(getBufferHR, "Failed to get swap chain buffer.\n"))
    {
        return false;
    }

    createDxgiSurfaceRenderTarget = d2dFactory->CreateDxgiSurfaceRenderTarget(
        dxgiBackBuffer.Get(), &props, d2dRenderTarget.GetAddressOf()
    );
    if (!TryDXCall(
            createDxgiSurfaceRenderTarget,
            "Failed to create Dxgi surface render target.\n"
        ))
    {
        return false;
    }
    return true;
}

bool TextRenderer::CreateDWriteFactory()
{
    HRESULT createFactoryHR;

    createFactoryHR = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown **>(dwriteFactory.GetAddressOf())
    );
    return TryDXCall(createFactoryHR, "Failed to create DWrite factory.\n");
}

bool TextRenderer::CreateTextFormat()
{
    HRESULT createTextFormatHR;

    createTextFormatHR = dwriteFactory->CreateTextFormat(
        L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 32.0f, L"en-us",
        textFormat.GetAddressOf()
    );

    return TryDXCall(createTextFormatHR, "Failed to create text format.\n");
}

bool TextRenderer::CreateSolidColourBrush()
{
    HRESULT createSolidBrushHR;

    createSolidBrushHR = d2dRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White, 1.0f), textBrush.GetAddressOf()
    );
    return TryDXCall(createSolidBrushHR, "Failed to create solid brush.\n");
}

bool D3DApp::Init(HWND hwnd, int screenWidth, int screenHeight)
{
    DeviceManager deviceManager = DeviceManager();
    ShaderManager shaderManager = ShaderManager();
    Renderer renderer = Renderer();

    if (!deviceManager.CreateDeviceAndSwapChain(
            hwnd, screenWidth, screenHeight
        ))
        return false;
    if (!renderer.CreateRenderTarget())
        return false;
    if (!renderer.SetRasteriserState())
        return false;
    if (!renderer.SetViewport(screenWidth, screenHeight))
        return false;
    if (!shaderManager.CompileShaders())
        return false;
    if (!shaderManager.CreateInputLayout())
        return false;
    return true;
}

void Renderer::ClearScreen(float clearColour[4])
{
    DeviceManager::Get().context->ClearRenderTargetView(
        renderTargetView.Get(), clearColour
    );
}

bool Renderer::RenderFrame(float clearColour[4], std::wstring fpsText)
{
    GameState gameState = GameState::Get();
    XMMATRIX world = gameState.shape.worldMatrix;
    XMMATRIX view = gameState.camera.viewMatrix;
    XMMATRIX projection = BufferManager::Get().projectionMatrix;

    this->ClearScreen(clearColour);
    this->DrawScreen(world, view, projection);
    if (!this->PresentScreen())
    {
        return false;
    }
    return true;
}

void Renderer::DrawScreen(
    DirectX::XMMATRIX world, DirectX::XMMATRIX view,
    DirectX::XMMATRIX projection
)
{
    DeviceManager &deviceManager = DeviceManager::Get();
    BufferManager &bufferManager = BufferManager::Get();
    ShaderManager &shaderManager = ShaderManager::Get();
    ComPtr<ID3D11DeviceContext> context = deviceManager.context;

    bufferManager.UpdateMatrixBuffer(world, view, projection);

    UINT stride = 3 * sizeof(float);
    UINT offset = 0;

    context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
    context->IASetInputLayout(shaderManager.inputLayout.Get());
    context->IASetVertexBuffers(
        0, 1, bufferManager.vertexBuffer.GetAddressOf(), &stride, &offset
    );
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->VSSetShader(shaderManager.vertexShader.Get(), nullptr, 0);
    context->VSSetConstantBuffers(
        0, 1, bufferManager.matrixBuffer.GetAddressOf()
    );
    context->PSSetShader(shaderManager.pixelShader.Get(), nullptr, 0);
    context->Draw(36, 0);
}

bool Renderer::PresentScreen()
{
    HRESULT presentHR;
    presentHR = DeviceManager::Get().swapChain->Present(1, 0);

    return TryDXCall(presentHR, "Present failed.\n");
}
