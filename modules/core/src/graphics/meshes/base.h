// Heavy inspiration from Paul Robertson's (University of Abertay) code
#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;
using namespace DirectX;

struct Vertex
{
    XMFLOAT3 positionCoordinate;
    XMFLOAT2 textureCoordinate;
    XMFLOAT3 normalVector;
};

class BaseMesh
{
    public:
    BaseMesh(
        ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
    );

    int nVertices = 0;

    virtual void
    sendData(D3D_PRIMITIVE_TOPOLOGY top = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    protected:
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> deviceContext;
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;

    virtual void initBuffers(ComPtr<ID3D11Device>);
};
