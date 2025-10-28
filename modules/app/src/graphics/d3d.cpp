#include <d3dcompiler.h>
#include <filesystem>
#include "spdlog/spdlog.h"
#include "d3d.h"
#include "utils.h"

D3D::D3D(Window *window)
{
    this->window = window;
}

bool D3D::createDeviceAndSwapChain()
{
    HRESULT createDeviceAndSwapChainHR;
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = this->window->spec.width;
    scd.BufferDesc.Height = this->window->spec.height;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = this->window->hwnd;
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
    float aspectRatio = static_cast<float>(this->window->spec.width) /
                        static_cast<float>(this->window->spec.height);
    this->projectionMatrix =
        XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, 0.1f, 100.0f);

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

    // TODO - move to a config file
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
    this->viewport.Width = static_cast<FLOAT>(this->window->spec.width);
    this->viewport.Height = static_cast<FLOAT>(this->window->spec.height);
    this->viewport.MinDepth = 0.0f;
    this->viewport.MaxDepth = 1.0f;

    this->deviceContext->RSSetViewports(1, &viewport);
    return true;
}

bool D3D::compileShaders()
{
    HRESULT compileVertexShaderHR;
    HRESULT compilePixelShaderHR;

    UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG |
                        D3DCOMPILE_SKIP_OPTIMIZATION;

    compileVertexShaderHR = D3DCompileFromFile(
        std::filesystem::absolute(L"modules\\app\\shaders\\vertex.hlsl")
            .c_str(),
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
        compileFlags, 0, this->vsBlob.GetAddressOf(),
        this->errorBlob.GetAddressOf()
    );
    if (!TryDXCall(compileVertexShaderHR, "Failed to compile vertex shader.\n"))
    {
        if (this->errorBlob)
        {
            spdlog::error((char *) this->errorBlob->GetBufferPointer());
        }
        return false;
    }

    compilePixelShaderHR = D3DCompileFromFile(
        std::filesystem::absolute(L"modules\\app\\shaders\\pixel.hlsl").c_str(),
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0",
        compileFlags, 0, this->psBlob.GetAddressOf(),
        this->errorBlob.GetAddressOf()
    );
    if (!TryDXCall(compilePixelShaderHR, "Failed to compile pixel shader.\n"))
    {
        if (this->errorBlob)
        {
            spdlog::error((char *) errorBlob->GetBufferPointer());
        }
        return false;
    }

    return true;
}

bool D3D::createShaders()
{
    HRESULT createVertexShaderHR;
    HRESULT createPixelShaderHR;

    createVertexShaderHR = this->device->CreateVertexShader(
        this->vsBlob->GetBufferPointer(), this->vsBlob->GetBufferSize(),
        nullptr, vertexShader.GetAddressOf()
    );
    if (!TryDXCall(createVertexShaderHR, "Failed to create vertex shader.\n"))
    {
        return false;
    }

    createPixelShaderHR = this->device->CreatePixelShader(
        this->psBlob->GetBufferPointer(), this->psBlob->GetBufferSize(),
        nullptr, this->pixelShader.GetAddressOf()
    );
    if (!TryDXCall(createPixelShaderHR, "Failed to create pixel shader.\n"))
    {
        return false;
    }

    return true;
}

bool D3D::createInputLayout()
{
    HRESULT createInputLayoutHR;
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    createInputLayoutHR = this->device->CreateInputLayout(
        layout, 1, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        this->inputLayout.GetAddressOf()
    );
    return TryDXCall(createInputLayoutHR, "Failed to create input layout.\n");
}

bool D3D::createMatrixBuffer()
{
    HRESULT createBufferHR;

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.ByteWidth = sizeof(XMMATRIX) * 3; // world, view, projection

    createBufferHR = this->device->CreateBuffer(
        &bufferDesc, nullptr, this->matrixBuffer.GetAddressOf()
    );
    return TryDXCall(
        createBufferHR, "Failed to create matrix constant buffer.\n"
    );
}

bool D3D::init()
{
    if (!this->createDeviceAndSwapChain())
        return false;
    if (!this->initProjectionMatrix())
        return false;
    if (!this->setRenderTargetView())
        return false;
    if (!this->setRasterizerState())
        return false;
    if (!this->setViewport())
        return false;
    if (!this->compileShaders())
        return false;
    if (!this->createShaders())
        return false;
    if (!this->createInputLayout())
        return false;
    if (!this->createMatrixBuffer())
        return false;
    return true;
}

bool D3D::updateMatrixBuffer(XMMATRIX worldMatrix, XMMATRIX viewMatrix)
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
        0, 1, matrixBuffer.GetAddressOf()
    );

    return true;
}

bool D3D::clearScreen(const float clearColour[4])
{
    this->deviceContext->ClearRenderTargetView(
        this->renderTargetView.Get(), clearColour
    );
    return true;
}

bool D3D::presentScreen()
{
    HRESULT presentHR;
    presentHR = this->swapChain->Present(1, 0);
    return TryDXCall(presentHR, "Present failed.\n");
}

bool PointLightBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(PointLightBuffer::ShaderBuffer);
    this->bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = 0;
    this->bufferDesc.StructureByteStride = 0;
    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create point light buffer."))
        return false;
    return true;
}

bool PointLightBuffer::setPixelShader()
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    PointLightBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Point light buffer pixel shader map failed.\n"))
        return false;
    bufferData = (PointLightBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->colour = pointLight->colour;
    bufferData->worldPosition = pointLight->worldPosition;
    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->PSSetConstantBuffers(
        0, 1, this->buffer.GetAddressOf()
    );
    return true;
}
