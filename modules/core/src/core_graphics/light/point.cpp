#include "point.h"
#include <DirectXMath.h>

// (Microsoft Copilot, 2025)
XMMATRIX
PointLight::calculateViewMatrix(XMVECTOR directionVector, XMVECTOR upVector)
{
    XMVECTOR worldPositionVector = XMLoadFloat(this->worldPosition.data());

    return XMMatrixLookAtLH(
        worldPositionVector, worldPositionVector + directionVector, upVector
    );
}

// (Microsoft Copilot, 2025)
XMMATRIX PointLight::calculateProjectionMatrix(float nearPlane, float farPlane)
{
    return XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.f, nearPlane, farPlane);
}

// (Robertson, n.d.)
// (Hughes, n.d.)
// (Falconer, n.d.)
XMMATRIX PointLight::calculateOrthographicProjectionMatrix(
    float screenWidth, float screenHeight, float nearPlane, float farPlane
)
{
    return XMMatrixOrthographicLH(
        screenWidth, screenHeight, nearPlane, farPlane
    );
}
