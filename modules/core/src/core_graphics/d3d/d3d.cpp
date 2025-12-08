#include <d3d11.h>
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
    const D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_12_0
    };

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

void D3D::setWireframeEnabled(bool isWireframeEnabled)
{
    if (isWireframeEnabled)
        this->setRasteriserState(this->wireframeEnabledRasteriserState);
    else
        this->setRasteriserState(this->defaultRasteriserState);
}

bool D3D::initProjectionMatrix()
{
    float aspectRatio = static_cast<float>(this->state->window->state->width) /
                        static_cast<float>(this->state->window->state->height);
    float nearPlane = 0.1f;
    float farPlane = 10000.f;
    this->projectionMatrix =
        XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, nearPlane, farPlane);

    this->orthographicProjectionMatrix = XMMatrixOrthographicLH(
        (float) this->state->window->state->width,
        (float) this->state->window->state->height,nearPlane, farPlane 
    );
    return true;
}

bool D3D::createBackBufferRenderTargetView()
{
    HRESULT getBufferHR;
    HRESULT createRenderTargetViewHR;
    ComPtr<ID3D11Texture2D> backBuffer;

    getBufferHR = this->swapChain->GetBuffer(
        0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void **>(backBuffer.GetAddressOf())
    );
    if (!TryDXCall(getBufferHR, "Failed to get back buffer from swap chain.\n"))
    {
        return false;
    }

    createRenderTargetViewHR = this->device->CreateRenderTargetView(
        backBuffer.Get(), nullptr, this->renderTargetView.GetAddressOf()
    );

    return true;
}

bool D3D::setRenderTargetView(
    ComPtr<ID3D11RenderTargetView> _renderTargetView,
    ComPtr<ID3D11DepthStencilView> _depthStencilView
)
{
    // THIS WAS NULL???
    this->deviceContext->OMSetRenderTargets(
        1, _renderTargetView.GetAddressOf(), _depthStencilView.Get()
    );
    return true;
}

bool D3D::setRenderTargetViewToBackBuffer()
{
    this->setRenderTargetView(this->renderTargetView, this->depthStencilView);
    return true;
}

bool D3D::setViewport(D3D11_VIEWPORT viewport)
{
    this->deviceContext->RSSetViewports(1, &viewport);
    return true;
}

bool D3D::createRasteriserStates()
{
    HRESULT createRasteriserHR;
    D3D11_RASTERIZER_DESC rasteriserDescription,
        wireframeEnabledRasteriserDescription;

    rasteriserDescription = {};
    rasteriserDescription.FillMode = D3D11_FILL_SOLID;
    rasteriserDescription.CullMode = D3D11_CULL_NONE;
    rasteriserDescription.FrontCounterClockwise = FALSE;

    createRasteriserHR = this->device->CreateRasterizerState(
        &rasteriserDescription, this->defaultRasteriserState.GetAddressOf()
    );
    if (!TryDXCall(createRasteriserHR, "Failed to create rasteriser.\n"))
    {
        return false;
    }

    wireframeEnabledRasteriserDescription = {};
    wireframeEnabledRasteriserDescription.FillMode = D3D11_FILL_WIREFRAME;
    wireframeEnabledRasteriserDescription.CullMode = D3D11_CULL_NONE;
    wireframeEnabledRasteriserDescription.FrontCounterClockwise = FALSE;

    createRasteriserHR = this->device->CreateRasterizerState(
        &wireframeEnabledRasteriserDescription,
        this->wireframeEnabledRasteriserState.GetAddressOf()
    );
    if (!TryDXCall(
            createRasteriserHR,
            "Failed to create wireframe enabled rasteriser.\n"
        ))
    {
        return false;
    }
    return true;
}

bool D3D::setRasteriserState(ComPtr<ID3D11RasterizerState> rasteriserState)
{
    this->deviceContext->RSSetState(rasteriserState.Get());
    return true;
}

bool D3D::setDefaultRasteriserState()
{
    this->setRasteriserState(this->defaultRasteriserState.Get());
    return true;
}

bool D3D::setDefaultViewport()
{
    // TODO - move to a config file
    this->viewport.TopLeftX = 0;
    this->viewport.TopLeftY = 0;
    this->viewport.Width =
        static_cast<FLOAT>(this->state->window->state->width);
    this->viewport.Height =
        static_cast<FLOAT>(this->state->window->state->height);
    this->viewport.MinDepth = 0.0f;
    this->viewport.MaxDepth = 1.0f;

    this->setViewport(this->viewport);
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
        std::string("Failed to read shader: ") + shaderFilepath;

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

ComPtr<ID3D11DomainShader> D3D::createDomainShader(ComPtr<ID3DBlob> shaderBlob)
{
    ComPtr<ID3D11DomainShader> domainShader;
    HRESULT createDomainShaderHR;

    createDomainShaderHR = this->device->CreateDomainShader(
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr,
        domainShader.GetAddressOf()
    );
    if (!TryDXCall(createDomainShaderHR, "Failed to create domain shader.\n"))
    {
        return nullptr;
    }
    return domainShader;
}

ComPtr<ID3D11HullShader> D3D::createHullShader(ComPtr<ID3DBlob> shaderBlob)
{
    ComPtr<ID3D11HullShader> hullShader;
    HRESULT createHullShaderHR;

    createHullShaderHR = this->device->CreateHullShader(
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr,
        hullShader.GetAddressOf()
    );
    if (!TryDXCall(createHullShaderHR, "Failed to create hull shader.\n"))
    {
        return nullptr;
    }
    return hullShader;
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

bool D3D::createDepthStencilEnabledState()
{
    HRESULT createDepthStencilStateHR;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    // Initialize the description of the stencil state.
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    createDepthStencilStateHR = this->device->CreateDepthStencilState(
        &depthStencilDesc, this->depthStencilStateDepthEnabled.GetAddressOf()
    );
    if (!TryDXCall(
            createDepthStencilStateHR,
            "Failed to create depth stencil enabled state"
        ))
        return false;
    return true;
}

bool D3D::createDepthStencilDisabledState()
{
    HRESULT createDepthStencilStateHR;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    // Initialize the description of the stencil state.
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.DepthEnable = false;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    createDepthStencilStateHR = this->device->CreateDepthStencilState(
        &depthStencilDesc, this->depthStencilStateDepthDisabled.GetAddressOf()
    );
    if (!TryDXCall(
            createDepthStencilStateHR,
            "Failed to create depth stencil enabled state"
        ))
        return false;
    return true;
}

void D3D::setDepthStencilState(bool isEnabled)
{
    if (isEnabled)
    {
        this->deviceContext->OMSetDepthStencilState(
            depthStencilStateDepthEnabled.Get(), 1
        );
        return;
    }
    this->deviceContext->OMSetDepthStencilState(
        depthStencilStateDepthDisabled.Get(), 1
    );
}

// TODO Lab code
bool D3D::setStencilBuffer()
{
    HRESULT createDepthStencilViewHR;

    // Initialise the depth stencil view.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    createDepthStencilViewHR = this->device->CreateDepthStencilView(
        this->depthStencilBuffer.Get(), &depthStencilViewDesc,
        this->depthStencilView.GetAddressOf()
    );
    if (!TryDXCall(
            createDepthStencilViewHR, "Failed to create depth stencil view\n"
        ))
        return false;
    this->deviceContext->OMSetRenderTargets(
        1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get()
    );

    return true;
}

bool D3D::init()
{
    if (!this->createDeviceAndSwapChain())
        return false;
    if (!this->initProjectionMatrix())
        return false;
    if (!this->createBackBufferRenderTargetView())
        return false;
    if (!this->setRenderTargetViewToBackBuffer())
        return false;
    if (!this->setDepthBuffer())
        return false;
    if (!this->createDepthStencilEnabledState())
        return false;
    if (!this->createDepthStencilDisabledState())
        return false;
    this->setDepthStencilState(true);
    if (!this->setStencilBuffer())
        return false;
    if (!this->createRasteriserStates())
        return false;
    if (!this->setDefaultRasteriserState())
        return false;
    if (!this->setDefaultViewport())
        return false;
    if (!this->createMatrixBuffer())
        return false;
    return true;
}

bool D3D::clearBackBuffer(const float clearColour[4])
{
    this->clearRenderTarget(
        clearColour, this->renderTargetView, this->depthStencilView
    );
    return true;
}

bool D3D::clearRenderTarget(
    const float clearColour[4],
    ComPtr<ID3D11RenderTargetView> _renderTargetView,
    ComPtr<ID3D11DepthStencilView> _depthStencilView
)
{
    this->deviceContext->ClearRenderTargetView(
        _renderTargetView.Get(), clearColour
    );
    this->deviceContext->ClearDepthStencilView(
        _depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0
    );
    return true;
}

bool D3D::presentScreen()
{
    HRESULT presentHR;
    presentHR = this->swapChain->Present(1, 0);
    return TryDXCall(presentHR, "Present failed.\n");
}
