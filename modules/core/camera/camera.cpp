#include "camera.h"
#include <DirectXMath.h>
#include <algorithm>

constexpr float PITCH_LIMIT = XM_PIDIV2 - 0.01f;

Camera::Camera()
{
    this->calculateViewMatrix();
}

XMMATRIX Camera::getCameraRotation()
{
    XMMATRIX yawMatrix = XMMatrixRotationY(yaw);
    XMMATRIX pitchMatrix = XMMatrixRotationX(pitch);
    XMMATRIX cameraRotation = pitchMatrix * yawMatrix;
    return cameraRotation;
}

void Camera::calculateViewMatrix()
{
    const XMVECTOR eye = this->eyePosition;

    XMMATRIX cameraRotation = this->getCameraRotation();
    XMVECTOR forward =
        XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), cameraRotation);
    XMVECTOR up =
        XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), cameraRotation);
    XMVECTOR at = XMVectorAdd(eye, forward);

    this->viewMatrix = XMMatrixLookAtLH(eye, at, up);
}

void Camera::move(XMVECTOR moveAmount)
{
    XMMATRIX cameraRotation = this->getCameraRotation();
    XMVECTOR worldMoveAmount =
        XMVector3TransformNormal(moveAmount, cameraRotation);
    this->eyePosition = XMVectorAdd(this->eyePosition, worldMoveAmount);
    this->calculateViewMatrix();
}

void Camera::rotate(float dYaw, float dPitch)
{
    this->pitch = std::clamp(pitch, -PITCH_LIMIT, PITCH_LIMIT);
    this->yaw = this->yaw + dYaw;
    this->pitch = this->pitch + dPitch;
    this->calculateViewMatrix();
}
