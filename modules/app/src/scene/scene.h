#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>
#include <vector>

#include "../graphics/d3d.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class TriangleMesh
{
    public:
    TriangleMesh(D3D *graphicsManager);

    std::vector<XMFLOAT3> vertexCoordinates;
    std::vector<unsigned int> vertexIndices;

    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> vertexIndexBuffer;

    int getVertexCount();
    int getVertexIndexCount();
    bool createBuffers(D3D *graphicsManager);

    void init();
};

class Camera
{
    public:
    Camera();
    XMMATRIX viewMatrix;
    XMVECTOR eyePosition = XMVectorSet(0.f, 0.f, -10.0f, 1.0f);
    // TODO - convert these to a single XMVECTOR
    float yaw = 0.0f;
    float pitch = 0.0f;

    XMMATRIX getCameraRotation();
};

class Scene
{
    public:
    TriangleMesh *mesh;
    XMMATRIX meshWorldMatrix;
    Camera *camera;

    Scene(D3D *graphicsManager);

    bool render();

    private:
    D3D *graphicsManager;
    bool clearScreen(const float clearColour[4]);
    bool drawScreen();
    bool presentScreen();
};
