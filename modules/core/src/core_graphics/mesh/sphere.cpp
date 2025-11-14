#include "sphere.h"

SphereMesh::SphereMesh(D3D *graphicsManager)
{
    this->init();
    this->createBuffers(graphicsManager);
    // this->vertices.clear();
    // this->vertexIndices.clear();
}

// Copilot help
void SphereMesh::init()
{
    this->vertices.clear();
    this->vertexIndices.clear();

    int resolution = 100;
    const float PI = 3.14159265359f;

    for (int y = 0; y <= resolution; ++y)
    {
        float v = float(y) / resolution;
        float theta = v * PI;

        for (int x = 0; x <= resolution; ++x)
        {
            float u = float(x) / resolution;
            float phi = u * 2.0f * PI;

            float xPos = std::sin(theta) * std::cos(phi);
            float yPos = std::cos(theta);
            float zPos = std::sin(theta) * std::sin(phi);

            Vertex newVertex = Vertex();
            newVertex.positionCoordinate = {xPos, yPos, zPos};
            newVertex.normalVector = {xPos, yPos, zPos};
            newVertex.textureCoordinate = {u, v};
            this->vertices.push_back(newVertex);
        }
    }

    for (int y = 0; y < resolution; ++y)
    {
        for (int x = 0; x < resolution; ++x)
        {
            int i0 = y * (resolution + 1) + x;
            int i1 = i0 + 1;
            int i2 = i0 + (resolution + 1);
            int i3 = i2 + 1;

            // First triangle
            this->vertexIndices.push_back(i0);
            this->vertexIndices.push_back(i2);
            this->vertexIndices.push_back(i1);

            // Second triangle
            this->vertexIndices.push_back(i1);
            this->vertexIndices.push_back(i2);
            this->vertexIndices.push_back(i3);
        }
    }
}
