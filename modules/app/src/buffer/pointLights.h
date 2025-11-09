#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/light/point.h>

#include <shared_shader/sharedbuffertypes.h>

class PointLightsBuffer : public BaseStructuredBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(std::vector<PointLight *> *pointLights);

    std::vector<PointLight *> *pointLights;
    struct ShaderBuffer : SharedBufferTypes::PointLightBufferType
    {
    };
};
