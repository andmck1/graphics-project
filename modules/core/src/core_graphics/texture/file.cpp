#include "file.h"
#include "core_utils/graphics.h"
#include <filesystem>
#include <spdlog/spdlog.h>
#include "WICTextureLoader.h"

// Some help from lab here
bool FileTexture::loadAndCreateTextureResource()
{
    HRESULT createTexture2DHR;

    spdlog::debug("Loading texture from file: " + filepath.string());

    createTexture2DHR = CreateWICTextureFromFile(
        this->graphicsManager->device.Get(),
        this->graphicsManager->deviceContext.Get(), filepath.wstring().c_str(),
        NULL, this->textureResource.GetAddressOf(), 0
    );

    return TryDXCall(createTexture2DHR, "Failed to create texture");
}

bool FileTexture::load()
{
    if (!this->loadAndCreateTextureResource())
        return false;
    return true;
}

bool FileTexture::setVertexShader(BufferSlots *slotCounters, int numViews)
{
    int startSlot = slotCounters->vertexShaderCounters.textureSlot;
    this->graphicsManager->deviceContext->VSSetShaderResources(
        startSlot, numViews, this->textureResource.GetAddressOf()
    );
    slotCounters->vertexShaderCounters.textureSlot++;

    return true;
}

bool FileTexture::setDomainShader(BufferSlots *slotCounters, int numViews)
{
    int startSlot = slotCounters->domainShaderCounters.textureSlot;
    this->graphicsManager->deviceContext->DSSetShaderResources(
        startSlot, numViews, this->textureResource.GetAddressOf()
    );
    slotCounters->domainShaderCounters.textureSlot++;

    return true;
}

bool FileTexture::setPixelShader(BufferSlots *slotCounters, int numViews)
{
    int startSlot = slotCounters->pixelShaderCounters.textureSlot;
    this->graphicsManager->deviceContext->PSSetShaderResources(
        startSlot, numViews, this->textureResource.GetAddressOf()
    );

    slotCounters->pixelShaderCounters.textureSlot++;
    return true;
}
