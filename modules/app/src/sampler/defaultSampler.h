#pragma once

#include "core_graphics/d3d/d3d.h"

class BaseSampler
{
    public:
    virtual ~BaseSampler() = default;
    virtual bool setPixelShader(D3D *graphicsManager) = 0;
};

class DefaultSampler : public BaseSampler
{
    public:
    ComPtr<ID3D11SamplerState> samplerState;
    bool init(D3D *graphicsManager);
    bool setPixelShader(D3D *graphicsManager) override;
};
