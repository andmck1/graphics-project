#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>

#include <shared_shader/sharedbuffertypes.h>

class TessellationHullDataBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(
            SharedBufferTypes::TessellationHullDataBufferType *settings
    );
    
    SharedBufferTypes::TessellationHullDataBufferType *settings;
    struct ShaderBuffer : SharedBufferTypes::TessellationHullDataBufferType
    {
    };
};
