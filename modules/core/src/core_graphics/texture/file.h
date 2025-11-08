#pragma once

#include "base.h"

#include "core_graphics/d3d/d3d.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <filesystem>

class FileTexture : public BaseTexture
{
    public:
    D3D *graphicsManager;
    std::filesystem::path filepath;

    ComPtr<ID3D11ShaderResourceView> textureResource;
    bool load() override;
    bool setVertexShader(int startSlot, int numberView) override;
    bool setPixelShader(int startSlot, int numberView) override;

    private:
    ComPtr<ID3D11Texture2D> texture;

    bool loadAndCreateTextureResource();
};
