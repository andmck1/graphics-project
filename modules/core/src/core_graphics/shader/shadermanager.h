#pragma once

#include <core_graphics/shader/shader.h>

#include <memory>

struct ShaderManagerConfig
{
    D3D *graphicsManager;
    std::string vertexShaderDirectory;
    std::string pixelShaderDirectory;
    std::vector<std::unique_ptr<BaseShader>> shaders;
};

class ShaderManager
{
    public:
    ShaderManager(ShaderManagerConfig *config);

    private:
    ShaderManagerConfig *config;

    bool loadShaders();
    bool createShaders();
};
