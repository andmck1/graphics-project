#include "triangle.h"

TriangleMesh::TriangleMesh(D3D *graphicsManager)
{
    this->init();
    this->createBuffers(graphicsManager);
}

void TriangleMesh::init()
{
    this->vertexCoordinates.push_back({0.0f, 1.0f, 0.0f});
    this->vertexIndices.push_back(0);
    this->vertexCoordinates.push_back({-1.0f, 0.0f, 0.0f});
    this->vertexIndices.push_back(1);
    this->vertexCoordinates.push_back({1.0f, 0.0f, 0.0f});
    this->vertexIndices.push_back(2);
}
