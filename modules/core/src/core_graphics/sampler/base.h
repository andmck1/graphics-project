#pragma once

#include "core_graphics/buffer/base.h"
#include <core_graphics/d3d/d3d.h>

class BaseSampler
{
    public:
    virtual ~BaseSampler() = default;
    virtual bool
    setVertexShader(D3D *graphicsManager, BufferSlots *slotCounters) = 0;
    virtual bool
    setDomainShader(D3D *graphicsManager, BufferSlots *slotCounters) = 0;
    virtual bool
    setPixelShader(D3D *graphicsManager, BufferSlots *slotCounters) = 0;
};
