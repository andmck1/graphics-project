#include "triangle.h"

TriangleVertices::TriangleVertices()
{
    this->vertices[0].positionCoordinate = XMFLOAT3(0.0f, 1.0f, 0.0f);
    this->vertices[0].textureCoordinate = XMFLOAT2(0.5f, 0.0f);
    this->vertices[0].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    this->vertices[1].positionCoordinate = XMFLOAT3(-1.0f, 0.0f, 0.0f);
    this->vertices[1].textureCoordinate = XMFLOAT2(0.0f, 1.0f);
    this->vertices[1].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    this->vertices[2].positionCoordinate = XMFLOAT3(1.0f, 0.0f, 1.0f);
    this->vertices[2].textureCoordinate = XMFLOAT2(1.0f, 1.0f);
    this->vertices[2].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    for (int i = 0; i < 3; i++)
    {
        this->indices[i] = i;
    }
}

TriangleMesh::TriangleMesh(
    ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
)
    : BaseMesh(device, deviceContext)
{
    initBuffers();
    this->nVertices = 3;
}

void TriangleMesh::initBuffers()
{
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    TriangleVertices triangleVertices = TriangleVertices();

    vertexBufferDesc = {
        sizeof(Vertex) * this->nVertices,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_VERTEX_BUFFER,
        0,
        0,
        0
    };
    vertexData = {triangleVertices.vertices, 0, 0};
    device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

    indexBufferDesc = {
        sizeof(unsigned long) * this->nVertices,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_INDEX_BUFFER,
        0,
        0,
        0
    };
    indexData = {triangleVertices.indices, 0, 0};
    device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
}
