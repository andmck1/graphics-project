#pragma once

#include "core_graphics/shader/shader.h"
#include <core_graphics/shadermanager/base.h>

#include "../shader/shader.h"

class ShaderManagerState : public BaseShaderManagerState
{
    public:
    ShaderStack defaultShaderStack;
    ShaderStack textureOnlyShaderStack;
    ShaderStack heightMappedObjectShaderStack;
    ShaderStack waveDataShaderStack;

    StandardVertexShader standardVertexShader;
    WaveVertexShader waveVertexShader;
    HeightMapVertexShader heightMapVertexShader;
    LightingPixelShader lightingPixelShader;
    TextureOnlyPixelShader textureOnlyPixelShader;

    MatrixBuffer matrixBuffer = MatrixBuffer();
    CameraBuffer cameraBuffer = CameraBuffer();
    WaveDataBuffer waveDataBuffer = WaveDataBuffer();
    AmbientLightBuffer ambientLightBuffer = AmbientLightBuffer();
    DirectionalLightsBuffer directionalLightsBuffer = DirectionalLightsBuffer();
    PointLightsBuffer pointLightsBuffer = PointLightsBuffer();
};
