#include "ortho.h"

OrthoMesh::OrthoMesh(
    D3D *graphicsManager, int height, int width, int xPosition, int yPosition
)
{
    this->height = height;
    this->width = width;
    this->xPosition = xPosition;
    this->yPosition = yPosition;

    this->init();
    this->createBuffers(graphicsManager);
}

void OrthoMesh::init()
{
    // (Robertson, n.d.)
    // (Hughes, n.d.)
    // (Falconer, n.d.)
    float left, right, top, bottom;
    left = ((float) this->width) / -2.f + ((float) this->xPosition);
    right = left + (float) this->width;
    top = ((float) this->height) / 2.f + ((float) this->yPosition);
    bottom = top - (float) this->height;

    for (int i = 0; i < 4; i++)
    {
        this->vertices.push_back(Vertex());
    }

    this->vertices[0].positionCoordinate = XMFLOAT3(left, bottom, 1.0f);
    this->vertices[0].textureCoordinate = XMFLOAT2(0.0f, 1.0f);
    this->vertices[0].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    this->vertices[1].positionCoordinate = XMFLOAT3(left, top, 1.0f);
    this->vertices[1].textureCoordinate = XMFLOAT2(0.0f, 0.0f);
    this->vertices[1].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    this->vertices[2].positionCoordinate = XMFLOAT3(right, top, 1.0f);
    this->vertices[2].textureCoordinate = XMFLOAT2(1.0f, 0.0f);
    this->vertices[2].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    this->vertices[3].positionCoordinate = XMFLOAT3(right, bottom, 1.0f);
    this->vertices[3].textureCoordinate = XMFLOAT2(1.0f, 1.0f);
    this->vertices[3].normalVector = XMFLOAT3(0.0f, 0.0f, -1.0f);

    this->vertexIndices.push_back(0);
    this->vertexIndices.push_back(2);
    this->vertexIndices.push_back(1);

    this->vertexIndices.push_back(0);
    this->vertexIndices.push_back(3);
    this->vertexIndices.push_back(2);
}
