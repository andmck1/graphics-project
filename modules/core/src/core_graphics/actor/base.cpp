#include "base.h"
#include <DirectXMathMatrix.inl>
#include <algorithm>

BaseActor::BaseActor()
{
    this->worldPosition = {0.f, 0.f, 0.f};
    this->scaling = {1.f, 1.f, 1.f};
    this->rotation = {0.f, 0.f, 0.f};
}

XMMATRIX BaseActor::getWorldMatrix()
{
    XMMATRIX worldMatrix = this->getScalingMatrix() *
                           this->getRotationMatrix() *
                           this->getTranslationMatrix();
    return worldMatrix;
}

XMMATRIX BaseActor::getTranslationMatrix()
{
    XMMATRIX worldPositionMatrix = XMMatrixTranslation(
        this->worldPosition[0], this->worldPosition[1], this->worldPosition[2]
    );
    return worldPositionMatrix;
}

XMMATRIX BaseActor::getRotationMatrix()
{
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(
        this->rotation[0], this->rotation[1], this->rotation[2]
    );
    return rotationMatrix;
}

XMMATRIX BaseActor::getScalingMatrix()
{
    XMMATRIX scalingMatrix =
        XMMatrixScaling(this->scaling[0], this->scaling[1], this->scaling[2]);
    return scalingMatrix;
}

void BaseActor::clampRotation()
{
    for (int i = 0; i < 2; i++)
    {
        this->rotation[i] =
            std::clamp(this->rotation[i], -g_ROTATION_LIMIT, g_ROTATION_LIMIT);
    }
}

void BaseActor::clampScaling()
{
    for (int i = 0; i < 2; i++)
    {
        this->scaling[i] = std::max(this->scaling[i], 0.f);
    }
}

void BaseActor::moveTo(
    float xWorldPosition, float yWorldPosition, float zWorldPosition
)
{
    this->worldPosition[0] = xWorldPosition;
    this->worldPosition[1] = yWorldPosition;
    this->worldPosition[2] = zWorldPosition;
}

void BaseActor::moveBy(float xOffset, float yOffset, float zOffset)
{
    this->worldPosition[0] += xOffset;
    this->worldPosition[1] += yOffset;
    this->worldPosition[2] += zOffset;
}

void BaseActor::rotateTo(float pitch, float yaw, float roll)
{
    this->rotation[0] = pitch;
    this->rotation[1] = yaw;
    this->rotation[2] = roll;
    this->clampRotation();
}

void BaseActor::rotateBy(float pitchOffset, float yawOffset, float rollOffset)
{
    this->rotation[0] += pitchOffset;
    this->rotation[1] += yawOffset;
    this->rotation[2] += rollOffset;
    this->clampRotation();
}

void BaseActor::scaleTo(float xFactor, float yFactor, float zFactor)
{
    this->scaling[0] = xFactor;
    this->scaling[1] = yFactor;
    this->scaling[2] = zFactor;
    this->clampScaling();
}

void BaseActor::scaleBy(
    float xFactorOffset, float yFactorOffset, float zFactorOffset
)
{
    this->scaling[0] += xFactorOffset;
    this->scaling[1] += yFactorOffset;
    this->scaling[2] += zFactorOffset;
    this->clampScaling();
}
