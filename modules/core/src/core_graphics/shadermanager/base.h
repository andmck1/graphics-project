#pragma once

#include <core_graphics/shader/shader.h>

#include <memory>

struct BaseShaderManagerState
{
    D3D *graphicsManager;
    std::string vertexShaderDirectory;
    std::string pixelShaderDirectory;
    std::vector<std::unique_ptr<BaseShader>> shaders;
};

class BaseShaderManager
{
    public:
    BaseShaderManager(BaseShaderManagerState *state);
    BaseShaderManagerState *state;

    private:
    bool loadShaders();
    bool createShaders();
};
