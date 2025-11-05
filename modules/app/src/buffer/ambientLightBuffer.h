#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/buffer.h>
#include <core_graphics/light/ambient.h>

#include <shared_shader/buffer.h>

class AmbientLightBuffer : public Buffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setPixelShader() override;

    AmbientLight *ambientLight;
    struct ShaderBuffer : LightShader::AmbientLightBuffer
    {
    };
};
