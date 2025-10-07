#include "default.h"
#include "graphics/meshes/triangle.h"

DefaultScene::DefaultScene(
    ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
)
    : BaseScene(device, deviceContext)
{
    TriangleMesh triangleMesh = TriangleMesh(this->device, this->deviceContext);
    XMMATRIX triangleWorldMatrix = XMMatrixIdentity();
    this->addMesh(triangleMesh, triangleWorldMatrix);
}
