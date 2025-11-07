#pragma once

#include "core_graphics/shader/shader.h"
#include <core_graphics/shadermanager/base.h>

class ShaderManagerState : public BaseShaderManagerState
{
    public:
    ShaderStack defaultShaderStack;
    ShaderStack lightSourceShaderStack;
    ShaderStack heightMappedObjectShaderStack;
    ShaderStack waveDataShaderStack;
};

class ShaderManager : public BaseShaderManager
{
};
