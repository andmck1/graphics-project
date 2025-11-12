#include "point.h"
#include <DirectXMath.h>

// Some copilot help here 
XMMATRIX
PointLight::calculateViewMatrix(XMVECTOR directionVector, XMVECTOR upVector)
{
    XMVECTOR worldPositionVector = XMLoadFloat(this->worldPosition.data());

    return XMMatrixLookAtLH(
        worldPositionVector, worldPositionVector + directionVector, upVector
    );
}

// Some copilot help here
XMMATRIX PointLight::calculateProjectionMatrix(float nearPlane, float farPlane)
{
    return XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.f, nearPlane, farPlane);
}

// Some lab code here
XMMATRIX PointLight::calculateOrthographicProjectionMatrix(
    float screenWidth, float screenHeight, float nearPlane, float farPlane
)
{
    return XMMatrixOrthographicLH(
        screenWidth, screenHeight, nearPlane, farPlane
    );
}
