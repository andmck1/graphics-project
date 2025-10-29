#pragma once

#include "../../graphics/d3d.h"
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
    // TODO - don't keep these in memory, delete after loading
    std::vector<Vertex> vertices;
    std::vector<unsigned int> vertexIndices;

    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> vertexIndexBuffer;

    int getVertexCount();
    int getVertexIndexCount();
    bool createBuffers(D3D *graphicsManager);

    virtual void init() {};
};
