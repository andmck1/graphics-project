#include <d3dcompiler.h>
#include <core_utils/graphics.h>
#include "d3d.h"

D3D::D3D(D3DAppState *state)
{
    this->state = state;
}

bool D3D::createDeviceAndSwapChain()
{
    HRESULT createDeviceAndSwapChainHR;
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = this->state->window->state->width;
    scd.BufferDesc.Height = this->state->window->state->height;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = this->state->window->hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0};

    createDeviceAndSwapChainHR = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevels, 1, D3D11_SDK_VERSION, &scd,
        this->swapChain.GetAddressOf(), this->device.GetAddressOf(),
        &featureLevel, this->deviceContext.GetAddressOf()
    );

    return TryDXCall(
        createDeviceAndSwapChainHR, "Failed to create device and swap chain.\n"
    );
}

void D3D::initProjectionMatrix()
{
    float aspectRatio = static_cast<float>(this->state->window->state->width) /
                        static_cast<float>(this->state->window->state->height);
    this->projectionMatrix =
        XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, 0.1f, 5000.0f);
}

bool D3D::createBackBufferRenderTargetView()
{
    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT getBufferHR;
    HRESULT createRenderTargetViewHR;

    getBufferHR = this->swapChain->GetBuffer(
        0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void **>(backBuffer.GetAddressOf())
    );
    if (!TryDXCall(getBufferHR, "Failed to get back buffer from swap chain.\n"))
    {
        return false;
    }

    createRenderTargetViewHR = this->device->CreateRenderTargetView(
        backBuffer.Get(), nullptr,
        this->backBufferRenderTargetView.GetAddressOf()
    );

    if (!TryDXCall(
            createRenderTargetViewHR, "Failed to create render target view."
        ))
        return false;

    return true;
}

bool D3D::setRenderTargetView(
    ComPtr<ID3D11RenderTargetView> renderTargetView,
    ComPtr<ID3D11DepthStencilView> depthStencilView
)
{
    this->deviceContext->OMSetRenderTargets(
        1, renderTargetView.GetAddressOf(), depthStencilView.Get()
    );
    return true;
}

bool D3D::setRenderTargetViewToBackBuffer()
{
    this->setRenderTargetView(
        this->backBufferRenderTargetView, this->backBufferDepthStencilView
    );
    return true;
}

bool D3D::setRasterizerState()
{
    HRESULT createRasteriserHR;

    this->rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    this->rasterizerDesc.CullMode = D3D11_CULL_NONE;
    this->rasterizerDesc.FrontCounterClockwise = FALSE;

    createRasteriserHR = this->device->CreateRasterizerState(
        &rasterizerDesc, this->rasterizerState.GetAddressOf()
    );
    if (!TryDXCall(createRasteriserHR, "Failed to create rasteriser.\n"))
    {
        return false;
    }
    this->deviceContext->RSSetState(this->rasterizerState.Get());
    return true;
}

void D3D::initDefaultViewport()
{
    // TODO - move to a config file
    this->defaultViewport.TopLeftX = 0;
    this->defaultViewport.TopLeftY = 0;
    this->defaultViewport.Width =
        static_cast<FLOAT>(this->state->window->state->width);
    this->defaultViewport.Height =
        static_cast<FLOAT>(this->state->window->state->height);
    this->defaultViewport.MinDepth = 0.0f;
    this->defaultViewport.MaxDepth = 1.0f;
}

bool D3D::setViewport(D3D11_VIEWPORT *viewport)
{
    this->deviceContext->RSSetViewports(1, viewport);
    return true;
}

ComPtr<ID3DBlob> D3D::loadShader(std::string shaderFilepath)
{
    HRESULT loadShaderHR;
    ComPtr<ID3DBlob> shaderBlob;

    const std::wstring shaderFilepathWString(
        shaderFilepath.begin(), shaderFilepath.end()
    );
    const std::string errorMessage =
        std::string("Failed to read pixel shader: ") + shaderFilepath;

    loadShaderHR = D3DReadFileToBlob(
        shaderFilepathWString.c_str(), shaderBlob.GetAddressOf()
    );
    if (!TryDXCall(loadShaderHR, errorMessage.c_str()))
        return nullptr;

    return shaderBlob;
}

ComPtr<ID3D11VertexShader> D3D::createVertexShader(ComPtr<ID3DBlob> shaderBlob)
{
    ComPtr<ID3D11VertexShader> vertexShader;
    HRESULT createVertexShaderHR;

    createVertexShaderHR = this->device->CreateVertexShader(
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr,
        vertexShader.GetAddressOf()
    );
    if (!TryDXCall(createVertexShaderHR, "Failed to create vertex shader.\n"))
    {
        return nullptr;
    }
    return vertexShader;
}

ComPtr<ID3D11PixelShader> D3D::createPixelShader(ComPtr<ID3DBlob> shaderBlob)
{
    ComPtr<ID3D11PixelShader> pixelShader;
    HRESULT createPixelShaderHR;

    createPixelShaderHR = this->device->CreatePixelShader(
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr,
        pixelShader.GetAddressOf()
    );
    if (!TryDXCall(createPixelShaderHR, "Failed to create pixel shader.\n"))
        return nullptr;
    return pixelShader;
}

ComPtr<ID3D11InputLayout> D3D::createInputLayout(
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescription,
    ComPtr<ID3DBlob> shaderBlob
)
{
    HRESULT createInputLayoutHR;
    ComPtr<ID3D11InputLayout> inputLayout;

    int numElements = inputElementDescription.size();
    createInputLayoutHR = this->device->CreateInputLayout(
        inputElementDescription.data(), numElements,
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
        inputLayout.GetAddressOf()
    );

    if (!TryDXCall(createInputLayoutHR, "Failed to create input layout.\n"))
        return nullptr;
    return inputLayout;
}

bool D3D::createMatrixBuffer()
{
    HRESULT createBufferHR;

    D3D11_BUFFER_DESC bufferDesc = {};
    // TODO - generalise this
    bufferDesc.ByteWidth = sizeof(XMMATRIX) * 3;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    createBufferHR = this->device->CreateBuffer(
        &bufferDesc, nullptr, this->matrixBuffer.GetAddressOf()
    );
    return TryDXCall(
        createBufferHR, "Failed to create matrix constant buffer.\n"
    );
}

// TODO Lab code
bool D3D::setDepthBuffer()
{
    HRESULT createDepthBufferHR;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    // Initialize the description of the depth buffer.
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Width = this->state->window->state->width;
    depthBufferDesc.Height = this->state->window->state->height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    createDepthBufferHR = this->device->CreateTexture2D(
        &depthBufferDesc, NULL, &(this->depthStencilBuffer)
    );
    return TryDXCall(createDepthBufferHR, "Failed to create depth buffer.\n");
}

bool D3D::createDepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDescription = {};
    HRESULT createDepthStencilStateHR;

    depthStencilStateDescription.DepthEnable = true;
    depthStencilStateDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDescription.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDescription.StencilEnable = true;
    depthStencilStateDescription.StencilReadMask = 0xFF;
    depthStencilStateDescription.StencilWriteMask = 0xFF;
    depthStencilStateDescription.FrontFace.StencilFailOp =
        D3D11_STENCIL_OP_KEEP;
    depthStencilStateDescription.FrontFace.StencilDepthFailOp =
        D3D11_STENCIL_OP_INCR;
    depthStencilStateDescription.FrontFace.StencilPassOp =
        D3D11_STENCIL_OP_KEEP;
    depthStencilStateDescription.FrontFace.StencilFunc =
        D3D11_COMPARISON_ALWAYS;
    depthStencilStateDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDescription.BackFace.StencilDepthFailOp =
        D3D11_STENCIL_OP_DECR;
    depthStencilStateDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    createDepthStencilStateHR = this->device->CreateDepthStencilState(
        &depthStencilStateDescription, &this->depthStencilState
    );
    if (!TryDXCall(
            createDepthStencilStateHR,
            "Failed to create depth stencil state view."
        ))
        return false;

    return true;
}

bool D3D::setDepthStencilState()
{
    this->deviceContext->OMSetDepthStencilState(
        this->depthStencilState.Get(), 1
    );
    return true;
}

void D3D::initDefaultDepthStencilDescription()
{
    this->defaultDepthStencilViewDescription.Format =
        DXGI_FORMAT_D24_UNORM_S8_UINT;
    this->defaultDepthStencilViewDescription.ViewDimension =
        D3D11_DSV_DIMENSION_TEXTURE2D;
    this->defaultDepthStencilViewDescription.Texture2D.MipSlice = 0;
}

bool D3D::createBackBufferStencilView()
{
    HRESULT createDepthStencilViewHR;

    createDepthStencilViewHR = this->device->CreateDepthStencilView(
        this->depthStencilBuffer.Get(), &defaultDepthStencilViewDescription,
        this->backBufferDepthStencilView.GetAddressOf()
    );
    if (!TryDXCall(
            createDepthStencilViewHR, "Failed to create depth stencil view\n"
        ))
        return false;
    return true;
}

bool D3D::init()
{
    if (!this->createDeviceAndSwapChain())
        return false;
    this->initProjectionMatrix();
    if (!this->createBackBufferRenderTargetView())
        return false;
    if (!this->setDepthBuffer())
        return false;
    this->initDefaultDepthStencilDescription();
    if (!this->createDepthStencilState())
        return false;
    if (!this->setDepthStencilState())
        return false;
    if (!this->createBackBufferStencilView())
        return false;
    if (!this->setRenderTargetViewToBackBuffer())
        return false;
    if (!this->setRasterizerState())
        return false;
    this->initDefaultViewport();
    if (!this->setViewport(&this->defaultViewport))
        return false;
    if (!this->createMatrixBuffer())
        return false;
    return true;
}

bool D3D::clearScreen(const float clearColour[4])
{
    this->deviceContext->ClearRenderTargetView(
        this->backBufferRenderTargetView.Get(), clearColour
    );
    this->deviceContext->ClearDepthStencilView(
        this->backBufferDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0
    );
    return true;
}

bool D3D::presentScreen()
{
    HRESULT presentHR;
    presentHR = this->swapChain->Present(1, 0);
    return TryDXCall(presentHR, "Present failed.\n");
}
