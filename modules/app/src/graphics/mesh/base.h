#pragma once

#include "../../graphics/d3d.h"
#include <d3d11.h>
#include <directxmath.h>
#include <vector>

class BaseMesh
{
    public:
    std::vector<XMFLOAT3> vertexCoordinates;
    std::vector<unsigned int> vertexIndices;

    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> vertexIndexBuffer;

    int getVertexCount();
    int getVertexIndexCount();
    bool createBuffers(D3D *graphicsManager);

    virtual void init() {};
};
