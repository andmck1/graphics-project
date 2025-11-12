#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/light/spotlight.h>

#include <shared_shader/sharedbuffertypes.h>

class SpotLightsBuffer : public BaseStructuredBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(std::vector<SpotLight *> *spotLights);

    std::vector<SpotLight *> *spotLights;
    struct ShaderBuffer : SharedBufferTypes::SpotLightBufferType
    {
    };
};
