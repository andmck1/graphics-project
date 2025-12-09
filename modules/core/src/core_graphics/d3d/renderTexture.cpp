#include "rendertexture.h"

#include "core_utils/graphics.h"

// (Robertson, n.d.)
// (Hughes, n.d.)
// (Falconer, n.d.)
void RenderTexture::init()
{
    D3D11_TEXTURE2D_DESC textureDesc {};
    HRESULT result;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

    // Setup the render target texture description.
    textureDesc.Width = this->textureWidth;
    textureDesc.Height = this->textureHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags =
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;
    textureDesc.SampleDesc.Quality = 0;
    // Create the render target texture.
    result = this->graphicsManager->device->CreateTexture2D(
        &textureDesc, NULL, this->renderTargetTexture.GetAddressOf()
    );
    TryDXCall(result, "Failed to create render target texture");

    // Setup the description of the render target view.
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    // Create the render target view.
    result = this->graphicsManager->device->CreateRenderTargetView(
        this->renderTargetTexture.Get(), &renderTargetViewDesc,
        this->renderTargetView.GetAddressOf()
    );
    TryDXCall(result, "Failed to create render target view");

    // Setup the description of the shader resource view.
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    // Create the shader resource view.
    result = this->graphicsManager->device->CreateShaderResourceView(
        this->renderTargetTexture.Get(), &shaderResourceViewDesc,
        this->shaderResourceView.GetAddressOf()
    );
    TryDXCall(result, "Failed to shader resource view");

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = this->textureWidth;
    depthBufferDesc.Height = this->textureHeight;
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
    result = this->graphicsManager->device->CreateTexture2D(
        &depthBufferDesc, NULL, this->depthStencilBuffer.GetAddressOf()
    );
    TryDXCall(result, "Failed to create depth stencil buffer.");

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = depthBufferDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    result = this->graphicsManager->device->CreateDepthStencilView(
        this->depthStencilBuffer.Get(), &depthStencilViewDesc,
        this->depthStencilView.GetAddressOf()
    );
    TryDXCall(result, "Failed to create depth stencil view.");

    // Setup the viewport for rendering.
    this->viewport.Width = (float) this->textureWidth;
    this->viewport.Height = (float) this->textureHeight;
    this->viewport.MinDepth = 0.0f;
    this->viewport.MaxDepth = 1.0f;
    this->viewport.TopLeftX = 0.0f;
    this->viewport.TopLeftY = 0.0f;

    // Setup the projection matrix.
    this->projectionMatrix = XMMatrixPerspectiveFovLH(
        ((float) XM_PI / 4.0f),
        ((float) this->textureWidth / (float) this->textureHeight),
        this->screenNear, this->screenFar
    );

    // Create an orthographic projection matrix for 2D rendering.
    this->orthographicProjectionMatrix = XMMatrixOrthographicLH(
        (float) this->textureWidth, (float) this->textureHeight,
        this->screenNear, this->screenFar
    );
}

void RenderTexture::clear(const float clearColour[4])
{
    this->graphicsManager->clearRenderTarget(
        clearColour, this->renderTargetView, this->depthStencilView
    );
}

bool RenderTexture::set()
{
    if (!this->graphicsManager->setRenderTargetView(
            this->renderTargetView, this->depthStencilView
        ))
        return false;
    if (!this->graphicsManager->setViewport(this->viewport))
        return false;

    return true;
}

bool RenderTexture::setPixelShader(BufferSlots *slotCounters)
{
    if (this->shaderResourceView == nullptr)
        return false;

    int startSlot = slotCounters->pixelShaderCounters.textureSlot;
    this->graphicsManager->deviceContext->PSSetShaderResources(
        startSlot, 1, this->shaderResourceView.GetAddressOf()
    );
    slotCounters->pixelShaderCounters.textureSlot++;
    return true;
}
