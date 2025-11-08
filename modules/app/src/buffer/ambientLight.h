#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/buffer.h>
#include <core_graphics/light/ambient.h>

#include "shared_shader/sharedbuffertypes.h"

class AmbientLightBuffer : public PixelBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setPixelShader(int bufferSlot, int numberOfBuffers) override;

    struct ShaderBuffer : SharedBufferTypes::AmbientLightBufferType
    {
    };

    AmbientLight *ambientLight;
};
