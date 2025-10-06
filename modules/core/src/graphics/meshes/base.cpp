#include "base.h"

BaseMesh::BaseMesh(
    ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
)
{
    this->device = device;
    this->deviceContext = deviceContext;
}

void BaseMesh::sendData(D3D_PRIMITIVE_TOPOLOGY top)
{
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(Vertex);
    offset = 0;

    this->deviceContext->IASetVertexBuffers(
        0, 1, &vertexBuffer, &stride, &offset
    );
    this->deviceContext->IASetIndexBuffer(
        indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    this->deviceContext->IASetPrimitiveTopology(top);
}
