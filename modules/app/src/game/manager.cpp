#include "manager.h"
#include "graphics/meshes/triangle.h"

GameManager::GameManager(
    ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
)
{
    this->scene = Scene();
    this->scene.meshes.push_back(TriangleMesh(device, deviceContext));
}
