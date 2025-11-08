#pragma once

#include "core_graphics/shader/shader.h"

#include "../buffer/matrix.h"
#include "../buffer/camera.h"
#include "../buffer/ambientLight.h"
#include "../buffer/pointLights.h"
#include "../buffer/directionalLights.h"
#include "../buffer/waveData.h"

class StandardVertexShader : public VertexShader
{
    public:
    MatrixBuffer matrixBuffer = MatrixBuffer();
    CameraBuffer cameraBuffer = CameraBuffer();
};

class HeightMapVertexShader : public VertexShader
{
    public:
    MatrixBuffer matrixBuffer = MatrixBuffer();
    CameraBuffer cameraBuffer = CameraBuffer();
};

class WaveVertexShader : public VertexShader
{
    public:
    MatrixBuffer matrixBuffer = MatrixBuffer();
    CameraBuffer cameraBuffer = CameraBuffer();
    WaveDataBuffer waveDataBuffer = WaveDataBuffer();
};

class LightingPixelShader : public PixelShader
{
    public:
    AmbientLightBuffer ambientLightBuffer = AmbientLightBuffer();
    PointLightsBuffer pointLightsBuffer = PointLightsBuffer();
    DirectionalLightsBuffer directionalLightsBuffer = DirectionalLightsBuffer();
};

class LightSourcePixelShader : public PixelShader
{
};
