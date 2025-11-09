#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/light/ambient.h>

#include "shared_shader/sharedbuffertypes.h"

class AmbientLightBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(AmbientLight *ambientLight);

    struct ShaderBuffer : SharedBufferTypes::AmbientLightBufferType
    {
    };

    AmbientLight *ambientLight;
};
