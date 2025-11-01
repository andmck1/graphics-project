#include "quad.h"

QuadMesh::QuadMesh(D3D *graphicsManager)
{
    this->init();
    this->createBuffers(graphicsManager);
}

void QuadMesh::init()
{
    for (int i = 0; i < 4; i++)
    {
        this->vertices.push_back(Vertex());
    }
    this->vertices[0].positionCoordinate = XMFLOAT3(-1.0f, -1.0f, 0.0f);
    this->vertices[0].textureCoordinate = XMFLOAT2(0.0f, 1.0f);
    this->vertices[0].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    this->vertices[1].positionCoordinate = XMFLOAT3(-1.0f, 1.0f, 0.0f);
    this->vertices[1].textureCoordinate = XMFLOAT2(0.0f, 0.0f);
    this->vertices[1].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    this->vertices[2].positionCoordinate = XMFLOAT3(1.0f, 1.0f, 0.0f);
    this->vertices[2].textureCoordinate = XMFLOAT2(1.0f, 0.0f);
    this->vertices[2].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    this->vertices[3].positionCoordinate = XMFLOAT3(1.0f, -1.0f, 0.0f);
    this->vertices[3].textureCoordinate = XMFLOAT2(1.0f, 1.0f);
    this->vertices[3].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);
    
    this->vertexIndices.push_back(0);
    this->vertexIndices.push_back(1);
    this->vertexIndices.push_back(2);
    this->vertexIndices.push_back(0);
    this->vertexIndices.push_back(3);
    this->vertexIndices.push_back(2);
}
