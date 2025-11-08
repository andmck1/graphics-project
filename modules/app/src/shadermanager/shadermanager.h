#pragma once

#include "core_graphics/shader/shader.h"
#include <core_graphics/shadermanager/base.h>

#include "../shader/shader.h"

class ShaderManagerState : public BaseShaderManagerState
{
    public:
    ShaderStack defaultShaderStack;
    ShaderStack lightSourceShaderStack;
    ShaderStack heightMappedObjectShaderStack;
    ShaderStack waveDataShaderStack;

    StandardVertexShader standardVertexShader;
    VertexShader waveVertexShader;
    VertexShader heightMapVertexShader;
    PixelShader lightingPixelShader;
    PixelShader lightSourcePixelShader;
};

class ShaderManager : public BaseShaderManager
{
};
