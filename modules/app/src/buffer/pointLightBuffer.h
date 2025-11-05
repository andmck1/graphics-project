#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/buffer.h>
#include <core_graphics/light/point.h>

#include <memory>
#include <shared_shader/buffer.h>

class PointLightBuffer : Buffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setPixelShader() override;

    PointLight *pointLight;
    struct ShaderBuffer : LightShader::LightBuffer
    {
    };
};
