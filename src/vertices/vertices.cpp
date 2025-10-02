#include "vertices.h"
#include "../utils/utils.h"

Shape::Shape()
{
    std::array<float, 108> initVertices = {
        // Front face
        -0.5f, -0.5f, 0.5f, // Bottom-left
        0.5f, -0.5f, 0.5f,  // Bottom-right
        0.5f, 0.5f, 0.5f,   // Top-right
        -0.5f, -0.5f, 0.5f, // Bottom-left
        0.5f, 0.5f, 0.5f,   // Top-right
        -0.5f, 0.5f, 0.5f,  // Top-left

        // Back face
        -0.5f, -0.5f, -0.5f, // Bottom-left
        -0.5f, 0.5f, -0.5f,  // Top-left
        0.5f, 0.5f, -0.5f,   // Top-right
        -0.5f, -0.5f, -0.5f, // Bottom-left
        0.5f, 0.5f, -0.5f,   // Top-right
        0.5f, -0.5f, -0.5f,  // Bottom-right

        // Left face
        -0.5f, -0.5f, -0.5f, // Bottom-left
        -0.5f, -0.5f, 0.5f,  // Bottom-right
        -0.5f, 0.5f, 0.5f,   // Top-right
        -0.5f, -0.5f, -0.5f, // Bottom-left
        -0.5f, 0.5f, 0.5f,   // Top-right
        -0.5f, 0.5f, -0.5f,  // Top-left

        // Right face
        0.5f, -0.5f, -0.5f, // Bottom-left
        0.5f, 0.5f, -0.5f,  // Top-left
        0.5f, 0.5f, 0.5f,   // Top-right
        0.5f, -0.5f, -0.5f, // Bottom-left
        0.5f, 0.5f, 0.5f,   // Top-right
        0.5f, -0.5f, 0.5f,  // Bottom-right

        // Top face
        -0.5f, 0.5f, -0.5f, // Bottom-left
        -0.5f, 0.5f, 0.5f,  // Bottom-right
        0.5f, 0.5f, 0.5f,   // Top-right
        -0.5f, 0.5f, -0.5f, // Bottom-left
        0.5f, 0.5f, 0.5f,   // Top-right
        0.5f, 0.5f, -0.5f,  // Top-left

        // Bottom face
        -0.5f, -0.5f, -0.5f, // Bottom-left
        0.5f, -0.5f, -0.5f,  // Bottom-right
        0.5f, -0.5f, 0.5f,   // Top-right
        -0.5f, -0.5f, -0.5f, // Bottom-left
        0.5f, -0.5f, 0.5f,   // Top-right
        -0.5f, -0.5f, 0.5f   // Top-left
    };
    this->vertices = initVertices;
    this->position = {0.0f, 0.0f, 0.0f};
    this->rotation = {0.0f, 0.0f, 0.0f};
}

void Shape::calculatePositionMatrix()
{
    std::array<float, 3> currentPosition = this->position;
    XMMATRIX translationMatrix = XMMatrixTranslation(
        currentPosition[0], currentPosition[1], currentPosition[2]
    );
    this->positionMatrix = translationMatrix;
}

void Shape::calculateRotationMatrix()
{
    std::array<float, 3> currentRotation = this->rotation;
    XMMATRIX rotX = XMMatrixRotationX(currentRotation[0]);
    XMMATRIX rotY = XMMatrixRotationY(currentRotation[1]);
    XMMATRIX rotZ = XMMatrixRotationZ(currentRotation[2]);
    XMMATRIX rotationMatrix = rotZ * rotY * rotX;
    this->rotationMatrix = rotationMatrix;
}

void Shape::calculateWorldMatrix()
{
    this->worldMatrix = this->rotationMatrix * this->positionMatrix;
}

size_t Shape::getVertexCount()
{
    return this->vertices.size();
}

void Shape::move(std::array<float, 3> moveAmount)
{
    std::array<float, 3> newPosition = this->position;
    newPosition[0] += moveAmount[0];
    newPosition[1] += moveAmount[1];
    newPosition[2] += moveAmount[2];
    this->position = newPosition;
    this->calculatePositionMatrix();
    this->calculateWorldMatrix();
}

void Shape::rotate(std::array<float, 3> rotateAmount)
{
    std::array<float, 3> newRotation = this->rotation;
    for (int i = 0; i < 3; ++i)
    {
        newRotation[i] += rotateAmount[i];
        if (newRotation[i] > TWO_PI)
            newRotation[i] -= TWO_PI;
        else if (newRotation[i] < -TWO_PI)
            newRotation[i] += TWO_PI;
    }
    this->rotation = newRotation;

    this->rotation = newRotation;
    this->calculateRotationMatrix();
    this->calculateWorldMatrix();
}
