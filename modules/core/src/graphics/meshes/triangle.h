#pragma once

#include "base.h"

struct TriangleVertices
{
    TriangleVertices();
    Vertex vertices[3];
    int indices[3];
};

class TriangleMesh : BaseMesh
{
    public:
    TriangleMesh(
        ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
    );

    protected:
    void initBuffers();
};
