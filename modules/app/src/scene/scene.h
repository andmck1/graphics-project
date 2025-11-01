#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/shader/shader.h>
#include <core_graphics/shader/shadermanager.h>
#include <core_graphics/camera/camera.h>
#include <core_graphics/mesh/triangle.h>
#include <core_graphics/mesh/sphere.h>
#include <core_graphics/light/point.h>
#include <core_graphics/mesh/quad.h>
#include <core_graphics/mesh/plane.h>
#include <core_graphics/mesh/ortho.h>

#include "../buffer/pointLightBuffer.h"

using namespace Microsoft::WRL;
using namespace DirectX;

struct SceneMesh
{
    BaseMesh *mesh;
    XMMATRIX worldMatrix;
    ShaderStack *shaderStack;
};

struct SceneConfig
{
    D3D *graphicsManager;
    ShaderManager *shaderManager;
    std::string vertexShaderDirectory;
    std::string pixelShaderDirectory;
};

class Scene
{
    public:
    std::vector<SceneMesh> meshes;

    PointLightBuffer pointLightBuffer;
    PointLight pointLight;

    Camera *camera;

    Scene(SceneConfig *config);

    bool drawMesh(UINT &stride, UINT &offset, SceneMesh *sceneMesh);
    bool render();

    private:
    D3D *graphicsManager;
    ShaderManager *shaderManager;
    bool drawScreen();
};

struct DebugSceneConfig
{
    D3D *graphicsManager;
    float *fps;
};

class DebugScene
{
    public:
    DebugScene(DebugSceneConfig *config);
    ~DebugScene();
    bool render();

    private:
    DebugSceneConfig *config;
};
