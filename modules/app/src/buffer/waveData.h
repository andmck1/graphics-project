#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/camera/camera.h>

#include <shared_shader/sharedbuffertypes.h>

class WaveDataBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(
        float *totalTime, float *magnitude, float *waveNumber,
        float *angularVelocity
    );

    float *totalTime;
    float *magnitude;
    float *waveNumber;
    float *angularVelocity;
    struct ShaderBuffer : SharedBufferTypes::WaveDataBufferType
    {
    };
};
