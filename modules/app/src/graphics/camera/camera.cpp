#include "camera.h"

#include <algorithm>

constexpr float gPITCH_LIMIT = XM_PIDIV2 - 0.01f;

Camera::Camera()
{
    this->update();
}

XMMATRIX Camera::getCameraRotation()
{
    XMMATRIX yawMatrix = XMMatrixRotationY(yaw);
    XMMATRIX pitchMatrix = XMMatrixRotationX(pitch);
    XMMATRIX cameraRotation = pitchMatrix * yawMatrix;
    return cameraRotation;
}

void Camera::update()
{
    XMMATRIX cameraRotation = getCameraRotation();
    XMVECTOR forward =
        XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), cameraRotation);
    XMVECTOR up =
        XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), cameraRotation);
    XMVECTOR at = XMVectorAdd(this->eyePosition, forward);

    this->viewMatrix = XMMatrixLookAtLH(this->eyePosition, at, up);
}

void Camera::move(float dx, float dy, float dz)
{
    XMVECTOR moveAmount = {dx, dy, dz};
    XMMATRIX cameraRotation = this->getCameraRotation();
    XMVECTOR worldMoveAmount =
        XMVector3TransformNormal(moveAmount, cameraRotation);
    this->eyePosition = XMVectorAdd(this->eyePosition, worldMoveAmount);
}

void Camera::rotate(float dYaw, float dPitch)
{
    this->yaw = this->yaw + dYaw;
    this->pitch = this->pitch + dPitch;
    this->pitch = std::clamp(pitch, -gPITCH_LIMIT, gPITCH_LIMIT);
}
