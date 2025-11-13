#include "base.h"
#include <core_utils/graphics.h>

int BaseMesh::getVertexCount()
{
    return this->vertices.size();
}

int BaseMesh::getVertexIndexCount()
{
    return this->vertexIndices.size();
}

bool BaseMesh::createBuffers(D3D *graphicsManager)
{
    HRESULT createBufferHR;
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    D3D11_BUFFER_DESC indexBufferDesc = {};
    D3D11_SUBRESOURCE_DATA vertexBufferData;
    D3D11_SUBRESOURCE_DATA indexBufferData;

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * this->getVertexCount();

    vertexBufferData.pSysMem = this->vertices.data();
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;

    createBufferHR = graphicsManager->device->CreateBuffer(
        &vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create vertex buffer.\n"))
    {
        return false;
    }

    indexBufferDesc.ByteWidth =
        sizeof(unsigned long) * this->getVertexIndexCount();
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    indexBufferData.pSysMem = this->vertexIndices.data();
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    createBufferHR = graphicsManager->device->CreateBuffer(
        &indexBufferDesc, &indexBufferData,
        this->vertexIndexBuffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create vertex buffer.\n"))
    {
        return false;
    }

    return true;
}

void BaseNonTessellatedMesh::sendMeshData(D3D *graphicsManager)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, this->vertexBuffer.GetAddressOf(), &stride, &offset
    );
    graphicsManager->deviceContext->IASetIndexBuffer(
        this->vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );
}

void Base3PointTessellatedMesh::sendMeshData(D3D *graphicsManager)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, this->vertexBuffer.GetAddressOf(), &stride, &offset
    );
    graphicsManager->deviceContext->IASetIndexBuffer(
        this->vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
    );
}


void Base4PointTessellatedMesh::sendMeshData(D3D *graphicsManager)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, this->vertexBuffer.GetAddressOf(), &stride, &offset
    );
    graphicsManager->deviceContext->IASetIndexBuffer(
        this->vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST
    );
}
