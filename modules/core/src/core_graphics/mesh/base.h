#pragma once

#include "../d3d/d3d.h"
#include <d3d11.h>
#include <directxmath.h>
#include <vector>

struct Vertex
{
    // TODO - give this a constructor
    XMFLOAT3 positionCoordinate;
    XMFLOAT2 textureCoordinate;
    XMFLOAT3 normalVector;
};

class BaseMesh
{
    public:
    virtual ~BaseMesh() = default;

    // TODO - don't keep these in memory, delete after loading
    std::vector<Vertex> vertices;
    std::vector<unsigned int> vertexIndices;

    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> vertexIndexBuffer;

    int getVertexCount();
    int getVertexIndexCount();
    virtual bool createBuffers(D3D *graphicsManager);

    virtual void init() = 0;
    virtual void sendMeshData(D3D *graphicsManager) = 0;
};

class BaseNonTessellatedMesh : public virtual BaseMesh
{
    public:
    virtual ~BaseNonTessellatedMesh() = default;

    void sendMeshData(D3D *graphicsManager) override;
};

class BaseTessellatedMesh : public virtual BaseMesh
{
    public:
    virtual ~BaseTessellatedMesh() = default;

    void sendMeshData(D3D *graphicsManager) override;
};
