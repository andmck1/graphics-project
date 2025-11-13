#pragma once

#include "core_graphics/buffer/base.h"
class BaseTexture
{
    public:
    virtual ~BaseTexture() = default;
    virtual bool load() = 0;
    virtual bool setVertexShader(BufferSlots *slotCounters, int numberView) = 0;
    virtual bool setDomainShader(BufferSlots *slotCounters, int numberView) = 0;
    virtual bool setPixelShader(BufferSlots *slotCounters, int numberView) = 0;
};
