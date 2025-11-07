#include "file.h"
#include "core_utils/graphics.h"
#include <filesystem>
#include <spdlog/spdlog.h>
#include "WICTextureLoader.h"

FileTexture::FileTexture(D3D *graphicsManager, std::string filepath)
{
    this->graphicsManager = graphicsManager;
    this->filepath = std::filesystem::absolute(std::filesystem::path(filepath));
}

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

bool FileTexture::setPixelShader(int startSlot, int numViews)
{
    this->graphicsManager->deviceContext->PSSetShaderResources(
        startSlot, numViews, this->textureResource.GetAddressOf()
    );

    return true;
}

bool FileTexture::setVertexShader(int startSlot, int numViews)
{
    this->graphicsManager->deviceContext->VSSetShaderResources(
        startSlot, numViews, this->textureResource.GetAddressOf()
    );

    return true;
}
