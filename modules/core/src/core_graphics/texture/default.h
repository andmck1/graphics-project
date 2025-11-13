#pragma once

#include "base.h"

#include "core_graphics/d3d/d3d.h"
#include <d3d11.h>
#include <wrl/client.h>

class DefaultTexture : public BaseTexture
{
    public:
    D3D *graphicsManager;
    uint32_t texturePixelColour;
    ComPtr<ID3D11ShaderResourceView> textureResource;

    bool load() override;
    bool setVertexShader(BufferSlots *slotCounters, int numberView) override;
    bool setDomainShader(BufferSlots *slotCounters, int numberView) override;
    bool setPixelShader(BufferSlots *slotCounters, int numberView) override;

    private:
    ComPtr<ID3D11Texture2D> texture;

    bool loadTexture();
    bool createResource();
};

