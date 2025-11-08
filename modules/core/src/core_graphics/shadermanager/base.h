#pragma once

#include <core_graphics/shader/shader.h>

struct BaseShaderManagerState
{
    D3D *graphicsManager;
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
