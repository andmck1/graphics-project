#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>

#include "../graphics/d3d.h"
#include "../graphics/camera/camera.h"
#include "../graphics/mesh/triangle.h"
#include "../graphics/mesh/sphere.h"
#include "../graphics/light/point.h"

using namespace Microsoft::WRL;
using namespace DirectX;

struct ShaderManager
{
    enum class ShaderType
    {
        STANDARD_VERTEX,
        LIGHTING_PIXEL
    };

    const std::unordered_map<std::string, ShaderType>
        shaderFilenameToTypeMapping = {
            {"vertex", ShaderType::STANDARD_VERTEX},
            {"pixel", ShaderType::LIGHTING_PIXEL}
    };

    std::unordered_map<ShaderType, ComPtr<ID3DBlob>> shaderBlobs;
};

struct SceneConfig
{
    D3D *graphicsManager;
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

    PointLightBuffer pointLightBuffer;
    PointLight pointLight;

    Camera *camera;

    Scene(SceneConfig *config);

    bool drawMesh(
        UINT &stride, UINT &offset, BaseMesh *mesh, XMMATRIX meshWorldMatrix
    );
    bool render();

    private:
    D3D *graphicsManager;
    ShaderManager shaderManager;
    bool drawScreen();
    bool loadVertexShaders(std::string vertexShaderDirectory);
    bool loadPixelShaders(std::string pixelShaderDirectory);
};

class DebugScene
{
    public:
    DebugScene(D3D *graphicsManager);
    ~DebugScene();
    bool render();

    private:
    D3D *graphicsManager;
};
