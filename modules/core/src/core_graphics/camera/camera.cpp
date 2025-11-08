#include "camera.h"

Camera::Camera()
{
    this->update();
}

void Camera::update()
{
    XMMATRIX cameraRotation = this->getRotationMatrix();
    XMVECTOR eyePosition = {
        this->worldPosition[0], this->worldPosition[1], this->worldPosition[2],
        1.f
    };
    XMVECTOR forward =
        XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), cameraRotation);
    XMVECTOR up =
        XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), cameraRotation);
    XMVECTOR at = XMVectorAdd(eyePosition, forward);

    this->viewMatrix = XMMatrixLookAtLH(eyePosition, at, up);
}
