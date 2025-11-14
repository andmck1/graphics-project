#pragma once

#include "core_graphics/shader/shader.h"
#include "../buffer/matrix.h"
#include "../buffer/camera.h"
#include "../buffer/ambientLight.h"
#include "../buffer/pointLights.h"
#include "../buffer/spotLights.h"
#include "../buffer/directionalLights.h"
#include "../buffer/waveData.h"
#include "../buffer/lightSettings.h"
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
    ShaderStack depthShaderStack;
    ShaderStack tessellatedWaterShaderStack;
    ShaderStack tessellatedTerrainShaderStack;
    ShaderStack tessellatedDepthWaterShaderStack;
    ShaderStack tessellatedDepthTerrainShaderStack;

    // Vertex
    StandardVertexShader standardVertexShader;
    DepthVertexShader depthVertexShader;
    RadialHeightMapVertexShader radialHeightMapVertexShader;
    TessellationPassThroughVertexShader tessellationPassThroughVertexShader;

    // Hull
    TessellatedQuadHullShader tessellatedQuadHullShader;

    // Domain
    TessellatedWaterDomainShader tessellatedWaterDomainShader;
    TessellatedTerrainDomainShader tessellatedTerrainDomainShader;
    TessellatedDepthWaterDomainShader tessellatedDepthWaterDomainShader;
    TessellatedDepthTerrainDomainShader tessellatedDepthTerrainDomainShader; 

    // Pixel
    LightingPixelShader lightingPixelShader;
    WaterLightingPixelShader waterLightingPixelShader;
    TextureOnlyPixelShader textureOnlyPixelShader;
    CRTPixelShader crtPixelShader;
    DepthPixelShader depthPixelShader;

    // Buffers
    MatrixVertexBuffer matrixVertexBuffer = MatrixVertexBuffer();
    MatrixHullBuffer matrixHullBuffer = MatrixHullBuffer();
    MatrixDomainBuffer matrixDomainBuffer = MatrixDomainBuffer();
    CameraVertexBuffer cameraVertexBuffer = CameraVertexBuffer();
    CameraHullBuffer cameraHullBuffer = CameraHullBuffer();
    CameraDomainBuffer cameraDomainBuffer = CameraDomainBuffer();
    WaveDataDomainBuffer waveDataDomainBuffer = WaveDataDomainBuffer();
    LightSettingsPixelBuffer lightSettingsPixelBuffer =
        LightSettingsPixelBuffer();
    TessellationHullDataBuffer tessellationHullDataBuffer =
        TessellationHullDataBuffer();
    AmbientLightBuffer ambientLightBuffer = AmbientLightBuffer();
    DirectionalLightsBuffer directionalLightsBuffer = DirectionalLightsBuffer();
    PointLightsBuffer pointLightsBuffer = PointLightsBuffer();
    SpotLightsBuffer spotLightsBuffer = SpotLightsBuffer();
    CRTBuffer crtBuffer = CRTBuffer();
};
