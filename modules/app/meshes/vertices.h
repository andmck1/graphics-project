#ifndef VERTICES_H
#define VERTICES_H

#include <array>
#include <directxmath.h>

using namespace DirectX;

class Shape
{
  public:
    Shape();

    std::array<float, 108> vertices;
    std::array<float, 3> position;
    std::array<float, 3> rotation;

    XMMATRIX positionMatrix = XMMatrixIdentity();
    XMMATRIX rotationMatrix = XMMatrixIdentity();
    XMMATRIX worldMatrix = XMMatrixIdentity();

    void calculatePositionMatrix();
    void calculateRotationMatrix();
    void calculateWorldMatrix();

    size_t getVertexCount();
    void move(std::array<float, 3> moveAmount);
    void rotate(std::array<float, 3> rotateAmount);
};

#endif
