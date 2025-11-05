#pragma once

#include "core_graphics/d3d/d3d.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <filesystem>

class BaseTexture
{
    public:
    virtual ~BaseTexture() = default;
    virtual bool load() = 0;
    virtual bool setPixelShader(int startSlot, int numberView) = 0;
};

class DefaultTexture : public BaseTexture
{
    public:
    DefaultTexture(D3D *graphicsManager, uint32_t texturePixelColour);
    ComPtr<ID3D11ShaderResourceView> textureResource;
    bool load() override;
    bool setPixelShader(int startSlot, int numberView) override;

    private:
    D3D *graphicsManager;
    uint32_t texturePixelColour;
    ComPtr<ID3D11Texture2D> texture;

    bool loadTexture();
    bool createResource();
};

class FileTexture : public BaseTexture
{
    public:
    FileTexture(D3D *graphicsManager, std::string filepath);
    ComPtr<ID3D11ShaderResourceView> textureResource;
    bool load() override;
    bool setPixelShader(int startSlot, int numberView) override;

    private:
    D3D *graphicsManager;
    uint32_t texturePixelColour;
    ComPtr<ID3D11Texture2D> texture;

    bool loadAndCreateTextureResource();

    std::filesystem::path filepath;
};
