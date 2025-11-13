#pragma once

#include "core_graphics/shader/shader.h"

#include "../buffer/matrix.h"
#include "../buffer/camera.h"
#include "../buffer/ambientLight.h"
#include "../buffer/pointLights.h"
#include "../buffer/spotLights.h"
#include "../buffer/directionalLights.h"
#include "../buffer/waveData.h"
#include "../buffer/crt.h"

// Vertex
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

class DepthVertexShader : public VertexShader
{
    public:
    MatrixBuffer matrixBuffer = MatrixBuffer();
};

class TessellationPassThroughVertexShader : public VertexShader
{
};

// Domain
class TessellatedTriangleDomainShader : public DomainShader
{
};

class TessellatedQuadDomainShader : public DomainShader
{
};

// Hull
class TessellatedTriangleHullShader : public HullShader
{
    public:
    MatrixBuffer matrixBuffer = MatrixBuffer();
};

class TessellatedQuadHullShader : public HullShader
{
    public:
    MatrixBuffer matrixBuffer = MatrixBuffer();
};

// Pixel
class LightingPixelShader : public PixelShader
{
    public:
    AmbientLightBuffer ambientLightBuffer = AmbientLightBuffer();
    PointLightsBuffer pointLightsBuffer = PointLightsBuffer();
    SpotLightsBuffer spotLightsBuffer = SpotLightsBuffer();
    DirectionalLightsBuffer directionalLightsBuffer = DirectionalLightsBuffer();
};

class TextureOnlyPixelShader : public PixelShader
{
};

class CRTPixelShader : public PixelShader
{
    public:
    CRTBuffer crtBuffer = CRTBuffer();
};

class DepthPixelShader : public PixelShader
{
};
