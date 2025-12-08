#include "directional.h"

XMMATRIX DirectionalLight::calculateViewMatrix()
{
    XMVECTOR worldPositionVector = XMVectorSet(0.f, 0.f, 0.f, 1.f);

    XMVECTOR _directionVector = XMVectorSet(
        directionVector[0], directionVector[1], directionVector[2], 0.f
    );

    if (XMVector3Equal(_directionVector, XMVectorZero()))
    {
        _directionVector = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    }
    _directionVector = XMVector3Normalize(_directionVector);

    // Set position to be far away from origin along direction
    float distance = 10000.f;
    worldPositionVector = XMVector3TransformCoord(
        worldPositionVector, XMMatrixTranslationFromVector(
                                 XMVectorScale(-_directionVector, distance)
                             )
    );

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

    return XMMatrixLookAtLH(worldPositionVector, XMVectorZero(), upVector);
}

// Some lab code here
XMMATRIX DirectionalLight::calculateOrthographicProjectionMatrix(
    float screenWidth, float screenHeight, float nearPlane, float farPlane
)
{
    return XMMatrixOrthographicLH(
        screenWidth, screenHeight, nearPlane, farPlane
    );
}
