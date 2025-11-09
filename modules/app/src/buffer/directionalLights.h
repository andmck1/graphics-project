#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/light/directional.h>

#include <shared_shader/sharedbuffertypes.h>

class DirectionalLightsBuffer : public BaseStructuredBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    void setData(std::vector<DirectionalLight *> *directionalLights);
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;

    std::vector<DirectionalLight *> *directionalLights;

    struct ShaderBuffer : SharedBufferTypes::DirectionalLightBufferType
    {
    };
};
