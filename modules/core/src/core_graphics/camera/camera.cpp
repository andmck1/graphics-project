#include "camera.h"
#include <DirectXMath.h>

Camera::Camera()
{
    // TODO - parameterise -10?
    XMVECTOR position = XMVectorSet(0.f, 0.f, -10.f, 1.f);
    XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
    XMVECTOR at = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    this->orthographicViewMatrix = XMMatrixLookAtLH(position, at, up);
    this->update();
}

void Camera::update()
{
    XMMATRIX cameraRotation = this->getRotationMatrix();
    XMVECTOR eyePosition = {
        this->worldPosition[0], this->worldPosition[1], this->worldPosition[2],
        1.f
    };
    XMVECTOR forward = XMVector3TransformNormal(
        XMVectorSet(0.f, 0.f, 1.f, 0.f), cameraRotation
    );
    XMVECTOR up = XMVector3TransformNormal(
        XMVectorSet(0.f, 1.f, 0.f, 0.f), cameraRotation
    );
    XMVECTOR at = XMVectorAdd(eyePosition, forward);

    this->viewMatrix = XMMatrixLookAtLH(eyePosition, at, up);
    
    this->updatePreviousWorldPosition();
    this->updatePreviousRotation();
    this->movementDirectionVector.x =
        this->previousForwardVector[0] - this->forwardVector[0];
    this->movementDirectionVector.y =
        this->previousForwardVector[1] - this->forwardVector[1];
}
