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
    ShaderStack depthShaderStack;
    ShaderStack tessellatedTriangleShaderStack;
    ShaderStack tessellatedQuadShaderStack;

    // Vertex
    StandardVertexShader standardVertexShader;
    WaveVertexShader waveVertexShader;
    HeightMapVertexShader heightMapVertexShader;
    DepthVertexShader depthVertexShader;
    TessellationPassThroughVertexShader tessellationPassThroughVertexShader;

    // Hull
    TessellatedTriangleHullShader tessellatedTriangleHullShader;
    TessellatedQuadHullShader tessellatedQuadHullShader;

    // Domain
    TessellatedTriangleDomainShader tessellatedTriangleDomainShader;
    TessellatedQuadDomainShader tessellatedQuadDomainShader;

    // Pixel
    LightingPixelShader lightingPixelShader;
    TextureOnlyPixelShader textureOnlyPixelShader;
    CRTPixelShader crtPixelShader;
    DepthPixelShader depthPixelShader;

    MatrixBuffer matrixBuffer = MatrixBuffer();
    MatrixDomainBuffer matrixDomainBuffer = MatrixDomainBuffer();
    CameraBuffer cameraBuffer = CameraBuffer();
    CameraHullBuffer cameraHullBuffer = CameraHullBuffer();
    WaveDataBuffer waveDataBuffer = WaveDataBuffer();
    AmbientLightBuffer ambientLightBuffer = AmbientLightBuffer();
    DirectionalLightsBuffer directionalLightsBuffer = DirectionalLightsBuffer();
    PointLightsBuffer pointLightsBuffer = PointLightsBuffer();
    SpotLightsBuffer spotLightsBuffer = SpotLightsBuffer();
    CRTBuffer crtBuffer = CRTBuffer();
};
