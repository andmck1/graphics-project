#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/buffer.h>
#include <core_graphics/light/directional.h>

#include <shared_shader/lightingBuffer.h>

class DirectionalLightsBuffer : public StructuredPixelBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setPixelShader(int bufferSlot, int numberOfBuffers) override;

    std::vector<std::unique_ptr<DirectionalLight>> *directionalLights;
    struct ShaderBuffer : LightingShader::DirectionalLightBufferType
    {
    };
};
