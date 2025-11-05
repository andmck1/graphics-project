#pragma once

#include "core_graphics/d3d/d3d.h"
#include <d3d11.h>
#include <wrl/client.h>

struct BaseTextureState
{
    D3D *graphicsManager;
};

// class FileTexture : BaseTexture
// {
//     public:
//     FileTexture(std::string filepath);
//
//     std::string filepath;
//
//     bool checkFileExists();
//     bool load(D3D *graphicsManager);
// };

class DefaultTexture
{
    public:
    DefaultTexture(D3D *graphicsManager, uint32_t texturePixelColour);
    ComPtr<ID3D11ShaderResourceView> textureResource;
    bool load();

    private:
    D3D *graphicsManager;
    uint32_t texturePixelColour;
    ComPtr<ID3D11Texture2D> texture;

    bool loadTexture();
    bool createResource();
};
