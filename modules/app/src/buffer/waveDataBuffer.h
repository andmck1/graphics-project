#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/buffer.h>
#include <core_graphics/camera/camera.h>

#include <shared_shader/waveBuffer.h>

class WaveDataBuffer : public VertexBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setVertexShader(int bufferSlot, int numberOfBuffers) override;

    float *totalTime;
    float *magnitude;
    float *waveNumber;
    float *angularVelocity;
    struct ShaderBuffer : WaveVertexShader::WaveDataBufferType
    {
    };
};
