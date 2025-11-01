#include "plane.h"

PlaneMesh::PlaneMesh(D3D *graphicsManager)
{
    this->init();
    this->createBuffers(graphicsManager);
}

void PlaneMesh::init()
{
    // Lab code
    int vertexCount, indexCount, index, i, j;
    float positionX, positionZ, u, v, increment;
    int resolution = 100;

    vertexCount = (resolution - 1) * (resolution - 1) * 6;

    indexCount = vertexCount;

    index = 0;
    u = 0;
    v = 0;
    increment = 1.0f / (float) resolution;

    for (j = 0; j < (resolution - 1); j++)
    {
        for (i = 0; i < (resolution - 1); i++)
        {

            // Upper left.
            positionX = (float) i;
            positionZ = (float) (j);

            this->vertices.push_back(Vertex());
            this->vertices[index].positionCoordinate =
                XMFLOAT3(positionX, 0.0f, positionZ);
            this->vertices[index].textureCoordinate = XMFLOAT2(u, v);
            this->vertices[index].normalVector = XMFLOAT3(0.0, 1.0, 0.0);
            this->vertexIndices.push_back(index);
            index++;

            // Upper right.
            positionX = (float) (i + 1);
            positionZ = (float) (j + 1);

            this->vertices.push_back(Vertex());
            this->vertices[index].positionCoordinate =
                XMFLOAT3(positionX, 0.0f, positionZ);
            this->vertices[index].textureCoordinate =
                XMFLOAT2(u + increment, v + increment);
            this->vertices[index].normalVector = XMFLOAT3(0.0, 1.0, 0.0);
            this->vertexIndices.push_back(index);
            index++;

            // lower left
            positionX = (float) (i);
            positionZ = (float) (j + 1);

            this->vertices.push_back(Vertex());
            this->vertices[index].positionCoordinate =
                XMFLOAT3(positionX, 0.0f, positionZ);
            this->vertices[index].textureCoordinate =
                XMFLOAT2(u, v + increment);
            this->vertices[index].normalVector = XMFLOAT3(0.0, 1.0, 0.0);
            this->vertexIndices.push_back(index);
            index++;

            // Upper left
            positionX = (float) (i);
            positionZ = (float) (j);

            this->vertices.push_back(Vertex());
            this->vertices[index].positionCoordinate =
                XMFLOAT3(positionX, 0.0f, positionZ);
            this->vertices[index].textureCoordinate = XMFLOAT2(u, v);
            this->vertices[index].normalVector = XMFLOAT3(0.0, 1.0, 0.0);
            this->vertexIndices.push_back(index);
            index++;

            // Bottom right
            positionX = (float) (i + 1);
            positionZ = (float) (j);

            this->vertices.push_back(Vertex());
            this->vertices[index].positionCoordinate =
                XMFLOAT3(positionX, 0.0f, positionZ);
            this->vertices[index].textureCoordinate =
                XMFLOAT2(u + increment, v);
            this->vertices[index].normalVector = XMFLOAT3(0.0, 1.0, 0.0);
            this->vertexIndices.push_back(index);
            index++;

            // Upper right.
            positionX = (float) (i + 1);
            positionZ = (float) (j + 1);

            this->vertices.push_back(Vertex());
            this->vertices[index].positionCoordinate =
                XMFLOAT3(positionX, 0.0f, positionZ);
            this->vertices[index].textureCoordinate =
                XMFLOAT2(u + increment, v + increment);
            this->vertices[index].normalVector = XMFLOAT3(0.0, 1.0, 0.0);
            this->vertexIndices.push_back(index);
            index++;

            u += increment;
        }

        u = 0;
        v += increment;
    }
}
