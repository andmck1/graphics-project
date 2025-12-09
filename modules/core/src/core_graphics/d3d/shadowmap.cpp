#include "shadowmap.h"

// (Robertson, n.d.)
// (Hughes, n.d.)
// (Falconer, n.d.)
void ShadowMap::init()
{
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = this->width;
    texDesc.Height = this->height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    this->graphicsManager->device->CreateTexture2D(
        &texDesc, 0, this->depthTexture.GetAddressOf()
    );

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Flags = 0;
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    this->graphicsManager->device->CreateDepthStencilView(
        this->depthTexture.Get(), &dsvDesc,
        this->depthStencilView.GetAddressOf()
    );

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    this->graphicsManager->device->CreateShaderResourceView(
        this->depthTexture.Get(), &srvDesc,
        this->shaderResourceView.GetAddressOf()
    );

    this->viewport.Width = this->width;
    this->viewport.Height = this->height;
    this->viewport.MinDepth = 0.0f;
    this->viewport.MaxDepth = 1.0f;
    this->viewport.TopLeftX = 0.0f;
    this->viewport.TopLeftY = 0.0f;

    this->nullRTVs[0].Reset();
}

bool ShadowMap::set()
{
    // (Microsoft Copilot, 2025)
    ComPtr<ID3D11ShaderResourceView>
        nullSRVs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
    this->graphicsManager->deviceContext->PSSetShaderResources(
        0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT,
        nullSRVs[0].GetAddressOf()
    );
    this->graphicsManager->deviceContext->RSSetViewports(1, &this->viewport);
    this->graphicsManager->deviceContext->OMSetRenderTargets(
        1, this->nullRTVs[0].GetAddressOf(), this->depthStencilView.Get()
    );
    this->graphicsManager->deviceContext->ClearDepthStencilView(
        this->depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0
    );
    return true;
}

bool ShadowMap::setPixelShader(BufferSlots *slotCounters)
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
