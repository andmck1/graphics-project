#pragma once

#include "graphics/meshes/base.h"
#include <DirectXMath.h>
#include <vector>

class BaseScene
{
    public:
    BaseScene(
        ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
    );
    std::vector<BaseMesh> meshes;
    std::vector<DirectX::XMMATRIX> worldMatrices;

    template <typename TMesh>
        requires(std::is_base_of_v<BaseMesh, TMesh>)
    int addMesh(TMesh newMesh, DirectX::XMMATRIX newWorldMatrix);

    void Render();

    protected:
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> deviceContext;
};
