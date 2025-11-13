#include "triangle.h"

void BaseTriangleMesh::init()
{
    for (int i=0; i<3; i++)
    {
        this->vertices.push_back(Vertex());
        this->vertexIndices.push_back(i);
    }
    this->vertices[0].positionCoordinate = {0.f, 1.f, 0.f};
    this->vertices[0].textureCoordinate = {0.5f, 0.f};
    this->vertices[0].normalVector = {0.f, 0.f, -1.f}; 

    this->vertices[1].positionCoordinate = {-1.f, 0.f, 0.f};
    this->vertices[1].textureCoordinate = {0.f, 1.f};
    this->vertices[1].normalVector = {0.f, 0.f, -1.f}; 

    this->vertices[2].positionCoordinate = {1.f, 0.f, 0.f};
    this->vertices[2].textureCoordinate = {1.f, 1.f};
    this->vertices[2].normalVector = {0.f, 0.f, -1.f}; 
}

TriangleMesh::TriangleMesh(D3D *graphicsManager)
{
    this->init();
    this->createBuffers(graphicsManager);
    // this->vertices.clear();
    // this->vertexIndices.clear();
}

TessellatedTriangleMesh::TessellatedTriangleMesh(D3D *graphicsManager)
{
    this->init();
    this->createBuffers(graphicsManager);
}

