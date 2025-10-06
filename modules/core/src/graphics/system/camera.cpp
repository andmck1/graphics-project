#include "camera.h"
#include <DirectXMath.h>
#include <algorithm>

constexpr float PITCH_LIMIT = XM_PIDIV2 - 0.01f;

Camera::Camera()
{
    calculateViewMatrix();
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
    const XMVECTOR eye = eyePosition;

    XMMATRIX cameraRotation = getCameraRotation();
    XMVECTOR forward =
        XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), cameraRotation);
    XMVECTOR up =
        XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), cameraRotation);
    XMVECTOR at = XMVectorAdd(eye, forward);

    viewMatrix = XMMatrixLookAtLH(eye, at, up);
}

void Camera::move(XMVECTOR moveAmount)
{
    XMMATRIX cameraRotation = getCameraRotation();
    XMVECTOR worldMoveAmount =
        XMVector3TransformNormal(moveAmount, cameraRotation);
    eyePosition = XMVectorAdd(eyePosition, worldMoveAmount);
    calculateViewMatrix();
}

void Camera::rotate(float dYaw, float dPitch)
{
    pitch = std::clamp(pitch, -PITCH_LIMIT, PITCH_LIMIT);
    yaw = yaw + dYaw;
    pitch = pitch + dPitch;
    calculateViewMatrix();
}
