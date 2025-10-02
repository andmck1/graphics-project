#include "game.h"
#include "../graphics/graphics.h"

GameState &GameState::Get()
{
    static GameState instance;
    return instance;
}

GameState::GameState()
{
    this->camera = Camera();
    this->shape = Shape();
}

double GameState::getDeltaTime() const
{
    return this->deltaTime;
}

void GameState::setDeltaTime(double deltaTime)
{
    this->deltaTime = deltaTime;
}

void GameState::moveAndUpdate(std::array<float, 3> moveCoordinates)
{
    BufferManager &bufferManager = BufferManager::Get();

    this->shape.move(moveCoordinates);
    XMMATRIX view = this->camera.viewMatrix;
    XMMATRIX projection = bufferManager.projectionMatrix;
    XMMATRIX worldMatrix = this->shape.worldMatrix;

    bufferManager.UpdateMatrixBuffer(worldMatrix, view, projection);
}

void GameState::rotateAndUpdate(std::array<float, 3> rotateAmount)
{

    BufferManager &bufferManager = BufferManager::Get();

    this->shape.rotate(rotateAmount);

    XMMATRIX view = this->camera.viewMatrix;
    XMMATRIX projection = bufferManager.projectionMatrix;
    XMMATRIX worldMatrix = this->shape.worldMatrix;

    bufferManager.UpdateMatrixBuffer(worldMatrix, view, projection);
}

void GameState::moveCameraAndUpdate(XMVECTOR moveAmount)
{
    BufferManager &bufferManager = BufferManager::Get();
    this->camera.move(moveAmount);

    XMMATRIX view = this->camera.viewMatrix;
    XMMATRIX projection = bufferManager.projectionMatrix;
    XMMATRIX worldMatrix = this->shape.worldMatrix;
    bufferManager.UpdateMatrixBuffer(worldMatrix, view, projection);
}

void GameState::rotateCameraAndUpdate(const float mouseDx, const float mouseDy)
{
    BufferManager &bufferManager = BufferManager::Get();
    constexpr float kSensitivity = 0.2f;
    float dYaw =
        mouseDx * static_cast<float>(this->getDeltaTime()) * kSensitivity;
    float dPitch =
        -mouseDy * static_cast<float>(this->getDeltaTime()) * kSensitivity;
    this->camera.rotate(dYaw, dPitch);

    XMMATRIX view = this->camera.viewMatrix;
    XMMATRIX projection = bufferManager.projectionMatrix;
    XMMATRIX worldMatrix = this->shape.worldMatrix;
    bufferManager.UpdateMatrixBuffer(worldMatrix, view, projection);
}
