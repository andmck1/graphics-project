#include "base.h"

BaseScene::BaseScene(
    ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
)
{
    this->device = device;
    this->deviceContext = deviceContext;
}

template <typename TMesh>
    requires(std::is_base_of_v<BaseMesh, TMesh>)
int BaseScene::addMesh(TMesh newMesh, DirectX::XMMATRIX newWorldMatrix)
{
    this->meshes.push_back(newMesh);
    this->worldMatrices.push_back(newWorldMatrix);

    return meshes.size() - 1;
}

void BaseScene::Render()
{
    for(int i=0; i<this->meshes.size(); i++)
    {
        this->meshes[i].sendData();
    }
}
