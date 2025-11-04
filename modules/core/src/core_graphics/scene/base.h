#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/shader/shader.h>
#include <core_graphics/shader/shadermanager.h>
#include <core_graphics/camera/camera.h>
#include <core_graphics/mesh/base.h>

using namespace Microsoft::WRL;
using namespace DirectX;

struct BaseSceneMesh
{
    BaseMesh *mesh;
    XMMATRIX worldMatrix;
    ShaderStack *shaderStack;
};

struct BaseSceneConfig
{
    D3D *graphicsManager;
    ShaderManager *shaderManager;
    std::string vertexShaderDirectory;
    std::string pixelShaderDirectory;
};

class BaseScene
{
    public:
    std::vector<BaseSceneMesh> meshes;
    ShaderStack shaderStack;

    Camera *camera;

    virtual bool
    drawMesh(UINT &stride, UINT &offset, BaseSceneMesh *sceneMesh) = 0;
    bool render();

    private:
    D3D *graphicsManager;
    ShaderManager *shaderManager;
    bool drawScreen();
};
