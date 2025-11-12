#include "spotlight.h"
#include <DirectXMath.h>

// Some lab code here
XMMATRIX
SpotLight::calculateViewMatrix()
{
    XMVECTOR worldPositionVector = XMLoadFloat(this->worldPosition.data());
    XMFLOAT3 direction;
    direction.x = this->directionVector[0];
    direction.y = this->directionVector[1];
    direction.z = this->directionVector[2];
    XMVECTOR _directionVector = XMVectorSet(direction.x, direction.y, direction.z, 1.f);

    XMVECTOR upVector = XMVectorSet(0.f, 1.f, 0.f, 1.f);
    XMVECTOR checkVector = XMVectorSet(0.f, 0.f, 1.f, 1.f);
    if (XMVector3NearEqual(
            _directionVector, checkVector, XMVectorSet(1e-5f, 1e-5, 1e-5, 1e-5)
        ))
        upVector = XMVectorSet(0.f, 0.f, 1.f, 1.f);
    else if (XMVector3Equal(_directionVector, -checkVector))
        upVector = XMVectorSet(0.f, 0.f, 1.f, 1.f);

    XMVECTOR rightVector = XMVector3Cross(_directionVector, upVector);
    upVector = XMVector3Cross(rightVector, _directionVector);

    return XMMatrixLookAtLH(
        worldPositionVector, worldPositionVector + _directionVector, upVector
    );
}

// Some copilot help here
XMMATRIX SpotLight::calculateProjectionMatrix(float nearPlane, float farPlane)
{
    return XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.f, nearPlane, farPlane);
}

// Some lab code here
XMMATRIX SpotLight::calculateOrthographicProjectionMatrix(
    float screenWidth, float screenHeight, float nearPlane, float farPlane
)
{
    return XMMatrixOrthographicLH(
        screenWidth, screenHeight, nearPlane, farPlane
    );
}
