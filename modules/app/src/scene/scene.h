#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/camera/camera.h>
#include <core_graphics/mesh/triangle.h>
#include <core_graphics/mesh/sphere.h>
#include <core_graphics/light/point.h>
#include <core_graphics/mesh/quad.h>
#include <core_graphics/mesh/plane.h>
#include <core_graphics/mesh/ortho.h>

#include "../shader/shader.h"
#include "../buffer/pointLightBuffer.h"

using namespace Microsoft::WRL;
using namespace DirectX;

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
    TriangleMesh *triangleMesh;
    XMMATRIX triangleMeshWorldMatrix;
    SphereMesh *sphereMesh;
    XMMATRIX sphereMeshWorldMatrix;
    QuadMesh *quadMesh;
    XMMATRIX quadMeshWorldMatrix;
    PlaneMesh *planeMesh;
    XMMATRIX planeMeshWorldMatrix;
    OrthoMesh *orthoMesh;
    XMMATRIX orthoMeshWorldMatrix;

    PointLightBuffer pointLightBuffer;
    PointLight pointLight;

    Camera *camera;

    Scene(SceneConfig *config);

    bool drawMesh(
        UINT &stride, UINT &offset, BaseMesh *mesh, XMMATRIX meshWorldMatrix,
        std::vector<ShaderManager::Shader> shaderStack
    );
    bool render();

    private:
    D3D *graphicsManager;
    ShaderManager *shaderManager;
    bool drawScreen();
    bool loadShadersFromDirectory(std::string shaderDirectory);
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
