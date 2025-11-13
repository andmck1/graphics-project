#pragma once

#include "core_graphics/shader/shader.h"
#include "../buffer/matrix.h"
#include "../buffer/camera.h"
#include "../buffer/ambientLight.h"
#include "../buffer/pointLights.h"
#include "../buffer/spotLights.h"
#include "../buffer/directionalLights.h"
#include "../buffer/waveData.h"
#include "../buffer/tessellationhulldata.h"
#include "../buffer/crt.h"
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
    MatrixHullBuffer matrixHullBuffer = MatrixHullBuffer();
    MatrixDomainBuffer matrixDomainBuffer = MatrixDomainBuffer();
    CameraVertexBuffer cameraVertexBuffer = CameraVertexBuffer();
    CameraHullBuffer cameraHullBuffer = CameraHullBuffer();
    CameraDomainBuffer cameraDomainBuffer = CameraDomainBuffer();
    WaveDataVertexBuffer waveDataVertexBuffer = WaveDataVertexBuffer();
    WaveDataDomainBuffer waveDataDomainBuffer = WaveDataDomainBuffer();
    TessellationHullDataBuffer tessellationHullDataBuffer =
        TessellationHullDataBuffer();
    AmbientLightBuffer ambientLightBuffer = AmbientLightBuffer();
    DirectionalLightsBuffer directionalLightsBuffer = DirectionalLightsBuffer();
    PointLightsBuffer pointLightsBuffer = PointLightsBuffer();
    SpotLightsBuffer spotLightsBuffer = SpotLightsBuffer();
    CRTBuffer crtBuffer = CRTBuffer();
};
