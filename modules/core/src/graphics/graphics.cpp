#include "graphics.h"

DeviceManager &DeviceManager::Get()
{
    static DeviceManager instance;
    return instance;
}
ShaderManager &ShaderManager::Get()
{
    static ShaderManager instance;
    return instance;
}
Renderer &Renderer::Get()
{
    static Renderer instance;
    return instance;
}
TextRenderer &TextRenderer::Get()
{
    static TextRenderer instance;
    return instance;
}
BufferManager &BufferManager::Get()
{
    static BufferManager instance;
    return instance;
}

bool BufferManager::InitMatrices(int width, int height)
{
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    projectionMatrix =
        XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, 0.1f, 100.0f);
    return true;
}

bool DeviceManager::CreateDeviceAndSwapChain(int width, int height)
{
    HRESULT createDeviceAndSwapChainHR;
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = AppState::Get().hwnd;
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
    DeviceManager &deviceManager = DeviceManager::Get();

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

bool BufferManager::CreateVertexBuffer()
{
    HRESULT createBufferHR;
    DeviceManager &deviceManager = DeviceManager::Get();
    float *vertices = GameState::Get().shape.vertices.data();

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.ByteWidth =
        sizeof(float) * GameState::Get().shape.getVertexCount();

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    vertexBufferData.pSysMem = vertices;
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;

    createBufferHR = deviceManager.device->CreateBuffer(
        &bufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf()
    );

    return TryDXCall(createBufferHR, "Failed to create vertex buffer.\n");
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

bool InitD3D(int width, int height)
{
    DeviceManager &deviceManager = DeviceManager::Get();
    ShaderManager &shaderManager = ShaderManager::Get();
    BufferManager &bufferManager = BufferManager::Get();
    Renderer &renderer = Renderer::Get();

    if (!deviceManager.CreateDeviceAndSwapChain(width, height))
        return false;
    if (!bufferManager.InitMatrices(width, height))
        return false;
    if (!renderer.CreateRenderTarget())
        return false;
    if (!renderer.SetRasteriserState())
        return false;
    if (!renderer.SetViewport(width, height))
        return false;
    if (!shaderManager.CompileShaders())
        return false;
    if (!shaderManager.CreateInputLayout())
        return false;
    if (!bufferManager.CreateMatrixBuffer())
        return false;
    if (!bufferManager.CreateVertexBuffer())
        return false;
    return true;
}

bool InitD2DResources()
{
    TextRenderer &textRenderer = TextRenderer::Get();
    if (!textRenderer.CreateD2D1Factory())
        return false;
    if (!textRenderer.CreateD2DRenderTarget())
        return false;
    if (!textRenderer.CreateDWriteFactory())
        return false;
    if (!textRenderer.CreateTextFormat())
        return false;
    if (!textRenderer.CreateSolidColourBrush())
        return false;
    return true;
}

bool TextRenderer::DrawTextOverlay(const std::wstring &text)
{
    HRESULT endDrawHR;
    D2D1_RECT_F layoutRect =
        D2D1::RectF(10, 10, 800, 600); // Where to draw the text

    d2dRenderTarget->BeginDraw();
    d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    d2dRenderTarget->DrawTextA(
        text.c_str(), (UINT32) text.length(), textFormat.Get(), &layoutRect,
        textBrush.Get()
    );
    endDrawHR = d2dRenderTarget->EndDraw();

    return TryDXCall(endDrawHR, "EndDraw Failed.\n");
}

bool BufferManager::CreateMatrixBuffer()
{
    HRESULT createBufferHR;

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.ByteWidth = sizeof(XMMATRIX) * 3; // world, view, projection

    createBufferHR = DeviceManager::Get().device->CreateBuffer(
        &bufferDesc, nullptr, matrixBuffer.GetAddressOf()
    );
    return TryDXCall(
        createBufferHR, "Failed to create matrix constant buffer.\n"
    );
}

bool BufferManager::UpdateMatrixBuffer(
    XMMATRIX world, XMMATRIX view, XMMATRIX projection
)
{
    HRESULT mapHR;
    DeviceManager &deviceManager = DeviceManager::Get();
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    mapHR = deviceManager.context->Map(
        matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Matrix buffer map failed.\n"))
    {
        return false;
    }

    // Transpose matrices (HLSL expects column-major by default)
    XMMATRIX *dataPtr = reinterpret_cast<XMMATRIX *>(mappedResource.pData);
    dataPtr[0] = XMMatrixTranspose(world);
    dataPtr[1] = XMMatrixTranspose(view);
    dataPtr[2] = XMMatrixTranspose(projection);

    deviceManager.context->Unmap(this->matrixBuffer.Get(), 0);

    // Bind to VS slot b0 (matches `register(b0)` in HLSL)
    deviceManager.context->VSSetConstantBuffers(
        0, 1, matrixBuffer.GetAddressOf()
    );

    return true;
}

void Renderer::ClearScreen(float clearColour[4])
{
    DeviceManager::Get().context->ClearRenderTargetView(
        renderTargetView.Get(), clearColour
    );
}

// TODO - change how this works - separating renderer and text renderer here
// makes this strange
bool Renderer::RenderFrame(float clearColour[4], std::wstring fpsText)
{
    GameState gameState = GameState::Get();
    XMMATRIX world = gameState.shape.worldMatrix;
    XMMATRIX view = gameState.camera.viewMatrix;
    XMMATRIX projection = BufferManager::Get().projectionMatrix;

    this->ClearScreen(clearColour);
    this->DrawScreen(world, view, projection);
    if (!TextRenderer::Get().DrawTextOverlay(fpsText))
    {
        return false;
    }
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

