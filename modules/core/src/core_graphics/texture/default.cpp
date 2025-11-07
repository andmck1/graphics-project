#include "default.h"
#include "core_utils/graphics.h"
#include <spdlog/spdlog.h>

DefaultTexture::DefaultTexture(
    D3D *graphicsManager, uint32_t texturePixelColour
)
{
    this->graphicsManager = graphicsManager;
    this->texturePixelColour = texturePixelColour;
}

// Some help from lab here
bool DefaultTexture::loadTexture()
{
    HRESULT createTexture2DHR;

    D3D11_SUBRESOURCE_DATA texture2DInitalData = {
        &this->texturePixelColour, sizeof(uint32_t), 0
    };

    D3D11_TEXTURE2D_DESC texture2DDescription = {};
    texture2DDescription.Width = texture2DDescription.Height =
        texture2DDescription.MipLevels = texture2DDescription.ArraySize = 1;
    texture2DDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2DDescription.SampleDesc.Count = 1;
    texture2DDescription.Usage = D3D11_USAGE_IMMUTABLE;
    texture2DDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    spdlog::debug("Loading default texture");
    createTexture2DHR = this->graphicsManager->device->CreateTexture2D(
        &texture2DDescription, &texture2DInitalData,
        this->texture.GetAddressOf()
    );
    return TryDXCall(createTexture2DHR, "Failed to create texture");
}

// Some help from lab here
bool DefaultTexture::createResource()
{
    HRESULT createShaderResourceViewHR;

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    spdlog::debug("Creating default texture resource");
    createShaderResourceViewHR =
        this->graphicsManager->device->CreateShaderResourceView(
            this->texture.Get(), &shaderResourceViewDesc,
            this->textureResource.GetAddressOf()
        );
    return TryDXCall(
        createShaderResourceViewHR, "Failed to create texture resource"
    );
}

bool DefaultTexture::load()
{
    if (!this->loadTexture())
        return false;
    if (!this->createResource())
        return false;
    return true;
}

bool DefaultTexture::setPixelShader(int startSlot, int numViews)
{
    this->graphicsManager->deviceContext->PSSetShaderResources(
        startSlot, numViews, this->textureResource.GetAddressOf()
    );

    return true;
}

bool DefaultTexture::setVertexShader(int startSlot, int numViews)
{
    this->graphicsManager->deviceContext->VSSetShaderResources(
        startSlot, numViews, this->textureResource.GetAddressOf()
    );

    return true;
}
