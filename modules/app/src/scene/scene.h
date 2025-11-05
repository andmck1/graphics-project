#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>
#include <array>

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/shader/shader.h>
#include <core_graphics/shader/shadermanager.h>
#include <core_graphics/camera/camera.h>
#include <core_graphics/mesh/triangle.h>
#include <core_graphics/mesh/sphere.h>
#include <core_graphics/light/ambient.h>
#include <core_graphics/light/point.h>
#include <core_graphics/mesh/quad.h>
#include <core_graphics/mesh/plane.h>
#include <core_graphics/mesh/ortho.h>
#include "core_graphics/mesh/base.h"
#include "core_graphics/texture/texture.h"

#include "../sampler/defaultSampler.h"
#include "../buffer/cameraBuffer.h"
#include "../buffer/ambientLightBuffer.h"
#include "../buffer/pointLightBuffer.h"

using namespace Microsoft::WRL;
using namespace DirectX;

struct SceneMesh
{
    std::unique_ptr<BaseMesh> mesh;
    XMMATRIX worldMatrix;
    ShaderStack *shaderStack;
    BaseTexture *texture;
    std::unique_ptr<BaseSampler> sampler;
};

struct SceneState
{
    D3D *graphicsManager;
    ShaderManager *shaderManager;
    std::string vertexShaderDirectory;
    std::string pixelShaderDirectory;
    std::array<float, 4> clearColour = {0.f, 0.f, 0.f, 0.f};

    Camera camera = Camera();
    ShaderStack shaderStack;
    std::vector<std::unique_ptr<SceneMesh>> meshes;
    std::unique_ptr<AmbientLight> ambientLight;
    std::unique_ptr<PointLight> pointLight;

    // buffers
    std::unique_ptr<CameraBuffer> cameraBuffer;
    std::unique_ptr<PointLightBuffer> pointLightBuffer;
    std::unique_ptr<AmbientLightBuffer> ambientLightBuffer;

};

class Scene
{
    public:
    Scene(SceneState *state);
    SceneState *state;

    bool drawMesh(UINT &stride, UINT &offset, SceneMesh *sceneMesh);
    bool drawScreen();
    bool render();
};
