#pragma once

#include <core_graphics/shader/shader.h>

#include <memory>

struct ShaderManagerState
{
    D3D *graphicsManager;
    std::string vertexShaderDirectory;
    std::string pixelShaderDirectory;
    std::vector<std::unique_ptr<BaseShader>> shaders;
};

class ShaderManager
{
    public:
    ShaderManager(ShaderManagerState *state);
    ShaderManagerState *state;

    private:

    bool loadShaders();
    bool createShaders();
};
