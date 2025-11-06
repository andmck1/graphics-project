#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/buffer.h>
#include <core_graphics/light/point.h>

#include <shared_shader/lightingBuffer.h>

class PointLightBuffer : public PixelBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setPixelShader(int bufferSlot, int numberOfBuffers) override;

    PointLight *pointLight;
    struct ShaderBuffer : LightingShader::PointLightBufferType
    {
    };
};

class PointLightsBuffer : public StructuredPixelBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setPixelShader(int bufferSlot, int numberOfBuffers) override;

    std::vector<std::unique_ptr<PointLight>> *pointLights;
    struct ShaderBuffer : LightingShader::PointLightBufferType
    {
    };
};
