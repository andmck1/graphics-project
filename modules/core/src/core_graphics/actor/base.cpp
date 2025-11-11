#include "base.h"
#include <DirectXMathMatrix.inl>
#include <algorithm>
#include <cstdlib>

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
        if (std::abs(rotation[i]) > g_ROTATION_LIMIT)
        {
            rotation[i] = 0;
        }
    }
}

void BaseActor::clampScaling()
{
    for (int i = 0; i < 2; i++)
    {
        this->scaling[i] = std::max(this->scaling[i], 0.f);
    }
}

void BaseActor::updatePreviousWorldPosition()
{
    this->previousWorldPosition[0] = this->worldPosition[0];
    this->previousWorldPosition[1] = this->worldPosition[1];
    this->previousWorldPosition[2] = this->worldPosition[2];
}

void BaseActor::updatePreviousRotation()
{
    this->previousRotation[0] = this->rotation[0];
    this->previousRotation[1] = this->rotation[1];
    this->previousRotation[2] = this->rotation[2];
}

void BaseActor::updatePreviousScaling()
{
    this->previousScaling[0] = this->scaling[0];
    this->previousScaling[1] = this->scaling[1];
    this->previousScaling[2] = this->scaling[2];
}

void BaseActor::updatePreviousForwardVector()
{
    this->previousForwardVector[0] = this->forwardVector[0];
    this->previousForwardVector[1] = this->forwardVector[1];
    this->previousForwardVector[2] = this->forwardVector[2];
}

void BaseActor::moveTo(
    float xWorldPosition, float yWorldPosition, float zWorldPosition
)
{
    this->updatePreviousWorldPosition();
    this->worldPosition[0] = xWorldPosition;
    this->worldPosition[1] = yWorldPosition;
    this->worldPosition[2] = zWorldPosition;
}

void BaseActor::moveBy(float xOffset, float yOffset, float zOffset)
{
    this->updatePreviousWorldPosition();
    this->worldPosition[0] += xOffset;
    this->worldPosition[1] += yOffset;
    this->worldPosition[2] += zOffset;
}

void BaseActor::moveRelativeToForwardVectorBy(
    float xOffset, float yOffset, float zOffset
)
{
    XMVECTOR _offsetVector = {xOffset, yOffset, zOffset};

    XMVECTOR _newWorldPosition =
        XMVector3TransformNormal(_offsetVector, this->getRotationMatrix());

    this->updatePreviousWorldPosition();
    this->worldPosition[0] += _newWorldPosition[0];
    this->worldPosition[1] += _newWorldPosition[1];
    this->worldPosition[2] += _newWorldPosition[2];
}

void BaseActor::rotateTo(float pitch, float yaw, float roll)
{

    this->updatePreviousRotation();
    this->rotation[0] = pitch;
    this->rotation[1] = yaw;
    this->rotation[2] = roll;
    this->clampRotation();

    XMVECTOR _initialForwardVector = {1.f, 0.f, 0.f};
    XMVECTOR _forwardVector = XMVector3TransformNormal(
        _initialForwardVector, XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
    );

    this->updatePreviousForwardVector();
    this->forwardVector[0] = _forwardVector[0];
    this->forwardVector[1] = _forwardVector[1];
    this->forwardVector[2] = _forwardVector[2];
}

void BaseActor::rotateBy(float pitchOffset, float yawOffset, float rollOffset)
{
    this->updatePreviousRotation();
    this->rotation[0] += pitchOffset;
    this->rotation[1] += yawOffset;
    this->rotation[2] += rollOffset;
    this->clampRotation();

    XMVECTOR _initialForwardVector = {
        this->forwardVector[0], this->forwardVector[1], this->forwardVector[2]
    };
    XMVECTOR _forwardVector = XMVector3TransformNormal(
        _initialForwardVector,
        XMMatrixRotationRollPitchYaw(
            this->rotation[0], this->rotation[1], this->rotation[2]
        )
    );

    this->updatePreviousForwardVector();
    this->forwardVector[0] = _forwardVector[0];
    this->forwardVector[1] = _forwardVector[1];
    this->forwardVector[2] = _forwardVector[2];
}

void BaseActor::scaleTo(float xFactor, float yFactor, float zFactor)
{
    this->updatePreviousScaling();
    this->scaling[0] = xFactor;
    this->scaling[1] = yFactor;
    this->scaling[2] = zFactor;
    this->clampScaling();
}

void BaseActor::scaleBy(
    float xFactorOffset, float yFactorOffset, float zFactorOffset
)
{
    this->updatePreviousScaling();
    this->scaling[0] += xFactorOffset;
    this->scaling[1] += yFactorOffset;
    this->scaling[2] += zFactorOffset;
    this->clampScaling();
}
