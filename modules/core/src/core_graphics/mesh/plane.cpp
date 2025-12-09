#include "plane.h"

PlaneMesh::PlaneMesh(D3D *graphicsManager)
{
    this->init();
    this->createBuffers(graphicsManager);
}

void PlaneMesh::init()
{
    // (Robertson, n.d.)
    // (Hughes, n.d.)
    // (Falconer, n.d.)
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

TessellatedPlaneMesh::TessellatedPlaneMesh(D3D *graphicsManager)
{
    this->init();
    this->createBuffers(graphicsManager);
}

void TessellatedPlaneMesh::init()
{
    int vertexIndex = 0;
    int resolution = 100;
    float uvResolution;
    XMFLOAT3 normal = XMFLOAT3(0.f, 1.f, 0.f);

    uvResolution = 1.0f / (float) resolution;

    for (int xIndex = 0; xIndex < (resolution - 1); xIndex++)
    {
        for (int zIndex = 0; zIndex < (resolution - 1); zIndex++)
        {
            float u, v;
            float xIndexFloat = (float) xIndex;
            float zIndexFloat = (float) zIndex;

            u = xIndex * uvResolution;
            v = zIndex * uvResolution;

            // Top left
            this->vertices.push_back(Vertex());
            this->vertices[vertexIndex].positionCoordinate =
                XMFLOAT3(xIndexFloat, 0.f, zIndexFloat);
            this->vertices[vertexIndex].textureCoordinate = XMFLOAT2(u, v);
            this->vertices[vertexIndex].normalVector = normal;
            this->vertexIndices.push_back(vertexIndex++);

            // Top right
            this->vertices.push_back(Vertex());
            this->vertices[vertexIndex].positionCoordinate =
                XMFLOAT3(xIndexFloat + 1.f, 0.f, zIndexFloat);
            this->vertices[vertexIndex].textureCoordinate =
                XMFLOAT2(u + uvResolution, v);
            this->vertices[vertexIndex].normalVector = normal;
            this->vertexIndices.push_back(vertexIndex++);

            // Bottom right
            this->vertices.push_back(Vertex());
            this->vertices[vertexIndex].positionCoordinate =
                XMFLOAT3(xIndexFloat + 1.f, 0.f, zIndexFloat + 1.f);
            this->vertices[vertexIndex].textureCoordinate =
                XMFLOAT2(u + uvResolution, v + uvResolution);
            this->vertices[vertexIndex].normalVector = normal;
            this->vertexIndices.push_back(vertexIndex++);

            // Bottom left
            this->vertices.push_back(Vertex());
            this->vertices[vertexIndex].positionCoordinate =
                XMFLOAT3(xIndexFloat, 0.f, zIndexFloat + 1.f);
            this->vertices[vertexIndex].textureCoordinate =
                XMFLOAT2(u, v + uvResolution);
            this->vertices[vertexIndex].normalVector = normal;
            this->vertexIndices.push_back(vertexIndex++);
        }
    }
}
