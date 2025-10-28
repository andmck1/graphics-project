#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>

#include "../graphics/d3d.h"
#include "../graphics/camera/camera.h"
#include "../graphics/mesh/triangle.h"
#include "../graphics/mesh/sphere.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class Scene
{
    public:
    TriangleMesh *triangleMesh;
    XMMATRIX triangleMeshWorldMatrix;
    SphereMesh *sphereMesh;
    XMMATRIX sphereMeshWorldMatrix;
    Camera *camera;

    Scene(D3D *graphicsManager);

    bool drawMesh(
        UINT &stride, UINT &offset, BaseMesh *mesh, XMMATRIX meshWorldMatrix
    );
    bool render();

    private:
    D3D *graphicsManager;
    bool drawScreen();
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
