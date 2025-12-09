#include "spotlight.h"
#include <directxmath.h>

// (Robertson, n.d.)
// (Hughes, n.d.)
// (Falconer, n.d.)
XMMATRIX SpotLight::calculateViewMatrix()
{
    XMVECTOR worldPositionVector =
        XMVectorSet(worldPosition[0], worldPosition[1], worldPosition[2], 1.f);

    XMVECTOR _directionVector = XMVectorSet(
        directionVector[0], directionVector[1], directionVector[2], 0.f
    );

    if (XMVector3Equal(_directionVector, XMVectorZero()))
    {
        _directionVector = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    }
    _directionVector = XMVector3Normalize(_directionVector);

    // check for gimball lock
    // TODO - use quaternions?
    XMVECTOR yVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);

    XMVECTOR upVector =
        fabs(XMVectorGetX(XMVector3Dot(_directionVector, yVector))) > 0.99f
            ? XMVectorSet(0.f, 0.f, 1.f, 0.f)
            : XMVectorSet(0.f, 1.f, 0.f, 0.f);
    XMVECTOR rightVector =
        XMVector3Normalize(XMVector3Cross(upVector, _directionVector));
    upVector = XMVector3Cross(_directionVector, rightVector);

    return XMMatrixLookAtLH(
        worldPositionVector, worldPositionVector + _directionVector, upVector
    );
}

// (Microsoft Copilot, 2025)
XMMATRIX SpotLight::calculateProjectionMatrix(float nearPlane, float farPlane)
{
    // cone angle to radian divided by 4
    return XMMatrixPerspectiveFovLH(
        this->coneAngle / XM_PI / 8.f, 1.f, nearPlane, farPlane
    );
}

// (Robertson, n.d.)
// (Hughes, n.d.)
// (Falconer, n.d.)
XMMATRIX SpotLight::calculateOrthographicProjectionMatrix(
    float screenWidth, float screenHeight, float nearPlane, float farPlane
)
{
    return XMMatrixOrthographicLH(
        screenWidth, screenHeight, nearPlane, farPlane
    );
}
