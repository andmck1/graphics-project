#pragma once

#include "base.h"

class WrapSampler : public BaseSampler
{
    public:
    ComPtr<ID3D11SamplerState> samplerState;
    bool init(D3D *graphicsManager);
    bool
    setVertexShader(D3D *graphicsManager, BufferSlots *slotCounters) override;
    bool
    setDomainShader(D3D *graphicsManager, BufferSlots *slotCounters) override;
    bool
    setPixelShader(D3D *graphicsManager, BufferSlots *slotCounters) override;
};
