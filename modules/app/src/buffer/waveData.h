#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/camera/camera.h>

#include <shared_shader/sharedbuffertypes.h>

class WaveDataVertexBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(
        float *totalTime, std::array<std::array<float, 3>, 2> *direction,
        std::array<float, 2> *magnitude, std::array<float, 2> *waveNumber,
        std::array<float, 2> *angularVelocity, std::array<float, 2> *steepness
    );

    float *totalTime;
    std::array<std::array<float, 3>, 2> *direction;
    std::array<float, 2> *magnitude;
    std::array<float, 2> *waveNumber;
    std::array<float, 2> *angularVelocity;
    std::array<float, 2> *steepness;
    struct ShaderBuffer : SharedBufferTypes::WaveDataBufferType
    {
    };
};

class WaveDataDomainBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(
        float *totalTime, std::array<std::array<float, 3>, 2> *direction,
        std::array<float, 2> *magnitude, std::array<float, 2> *waveNumber,
        std::array<float, 2> *angularVelocity, std::array<float, 2> *steepness
    );

    float *totalTime;
    std::array<std::array<float, 3>, 2> *direction;
    std::array<float, 2> *magnitude;
    std::array<float, 2> *waveNumber;
    std::array<float, 2> *angularVelocity;
    std::array<float, 2> *steepness;
    struct ShaderBuffer : SharedBufferTypes::WaveDataBufferType
    {
    };
};
