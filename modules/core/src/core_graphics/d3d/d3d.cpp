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

bool D3D::initProjectionMatrix()
{
    float aspectRatio = static_cast<float>(this->state->window->state->width) /
                        static_cast<float>(this->state->window->state->height);
    this->projectionMatrix =
        XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, 0.1f, 5000.0f);

    return true;
}

bool D3D::setRenderTargetView()
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

    this->deviceContext->OMSetRenderTargets(
        1, renderTargetView.GetAddressOf(), nullptr
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

bool D3D::setViewport()
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

    this->deviceContext->RSSetViewports(1, &viewport);
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

// TODO Lab code
bool D3D::setStencilBuffer()
{
    HRESULT createDepthStencilViewHR;
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
    this->device->CreateDepthStencilState(
        &depthStencilDesc, &depthStencilState
    );
    this->deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

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
    if (!this->setRenderTargetView())
        return false;
    if (!this->setDepthBuffer())
        return false;
    if (!this->setStencilBuffer())
        return false;
    if (!this->setRasterizerState())
        return false;
    if (!this->setViewport())
        return false;
    if (!this->createMatrixBuffer())
        return false;
    return true;
}

bool D3D::updateMatrixBuffer(
    XMMATRIX worldMatrix, XMMATRIX viewMatrix, int bufferSlot,
    int numberOfBuffers
)
{
    HRESULT mapHR;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    mapHR = this->deviceContext->Map(
        matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Matrix buffer map failed.\n"))
    {
        return false;
    }

    XMMATRIX *dataPtr = reinterpret_cast<XMMATRIX *>(mappedResource.pData);
    dataPtr[0] = XMMatrixTranspose(worldMatrix);
    dataPtr[1] = XMMatrixTranspose(viewMatrix);
    dataPtr[2] = XMMatrixTranspose(this->projectionMatrix);

    this->deviceContext->Unmap(this->matrixBuffer.Get(), 0);
    this->deviceContext->VSSetConstantBuffers(
        bufferSlot, numberOfBuffers, matrixBuffer.GetAddressOf()
    );

    return true;
}

bool D3D::clearScreen(const float clearColour[4])
{
    this->deviceContext->ClearRenderTargetView(
        this->renderTargetView.Get(), clearColour
    );
    this->deviceContext->ClearDepthStencilView(
        this->depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0
    );
    return true;
}

bool D3D::presentScreen()
{
    HRESULT presentHR;
    presentHR = this->swapChain->Present(1, 0);
    return TryDXCall(presentHR, "Present failed.\n");
}
