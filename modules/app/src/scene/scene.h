#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>
#include <array>

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/shader/shader.h>
#include <core_graphics/camera/camera.h>
#include <core_graphics/mesh/triangle.h>
#include <core_graphics/mesh/sphere.h>
#include <core_graphics/light/ambient.h>
#include <core_graphics/light/point.h>
#include <core_graphics/light/directional.h>
#include <core_graphics/mesh/quad.h>
#include <core_graphics/mesh/plane.h>
#include <core_graphics/mesh/ortho.h>
#include <core_graphics/mesh/base.h>
#include <core_graphics/texture/default.h>
#include <core_graphics/texture/file.h>
#include <core_graphics/scene/base.h>
#include <core_graphics/sampler/base.h>
#include <core_graphics/sampler/clamp.h>
#include <core_graphics/sampler/wrap.h>
#include <core_utils/timer.h>

#include "../shadermanager/shadermanager.h"
#include "../buffer/cameraBuffer.h"
#include "../buffer/ambientLightBuffer.h"
#include "../buffer/pointLightBuffer.h"
#include "../buffer/directionalLightBuffer.h"
#include "../buffer/waveDataBuffer.h"
#include "../texture/textureManager.h"
#include "core_graphics/water/water.h"

struct SceneMesh : BaseSceneMesh
{
};

struct SceneState
{
    D3D *graphicsManager;
    ShaderManager *shaderManager;
    TextureManager *textureManager;
    std::string vertexShaderDirectory;
    std::string pixelShaderDirectory;
    std::array<float, 4> clearColour = {0.f, 0.f, 0.f, 0.f};

    Camera camera = Camera();
    // TODO - link up to app timer
    Timer timer = Timer();

    std::unique_ptr<ClampSampler> clampSampler;
    std::unique_ptr<WrapSampler> wrapSampler;
    std::vector<std::unique_ptr<SceneMesh>> meshes;

    std::unique_ptr<AmbientLight> ambientLight;
    std::unique_ptr<AmbientLightBuffer> ambientLightBuffer;

    std::vector<std::unique_ptr<PointLight>> pointLights;
    std::unique_ptr<PointLightsBuffer> pointLightsBuffer;

    std::vector<std::unique_ptr<DirectionalLight>> directionalLights;
    std::unique_ptr<DirectionalLightsBuffer> directionalLightsBuffer;
    
    std::unique_ptr<Water> water;
    std::unique_ptr<WaveDataBuffer> waveDataBuffer;

    std::unique_ptr<CameraBuffer> cameraBuffer;
};

class Scene : BaseScene
{
    public:
    Scene(SceneState *state);
    SceneState *state;

    bool drawMesh(UINT &stride, UINT &offset, SceneMesh *sceneMesh);
    void updateMeshPositions();
    bool drawScreen();
    bool render();
};
