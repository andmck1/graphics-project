#pragma once

#include "base.h"

class DefaultSampler : public BaseSampler
{
    public:
    ComPtr<ID3D11SamplerState> samplerState;
    bool init(D3D *graphicsManager);
    bool setPixelShader(D3D *graphicsManager) override;
};
