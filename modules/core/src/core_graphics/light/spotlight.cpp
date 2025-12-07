#include "spotlight.h"
#include <directxmath.h>

// Some lab code here
XMMATRIX SpotLight::calculateViewMatrix()
{
    // Load position and direction
    XMVECTOR pos =
        XMVectorSet(worldPosition[0], worldPosition[1], worldPosition[2], 1.f);

    XMVECTOR dir = XMVectorSet(
        directionVector[0], directionVector[1], directionVector[2], 0.f
    );

    // Handle zero direction vector (0,0,0)
    if (XMVector3Equal(dir, XMVectorZero()))
    {
        // Default forward direction
        dir = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    }

    // Normalize direction
    dir = XMVector3Normalize(dir);

    // Choose up vector
    XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

    // If forward is too close to +Y or -Y, pick another up vector
    XMVECTOR yAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
    float dot = XMVectorGetX(XMVector3Dot(dir, yAxis));

    if (fabs(dot) > 0.99f) // almost parallel to Y
    {
        up = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    }

    // Recompute right and corrected up
    XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, dir));
    up = XMVector3Cross(dir, right);

    // Build view matrix
    return XMMatrixLookAtLH(pos, pos + dir, up);
}

// Some copilot help here
XMMATRIX SpotLight::calculateProjectionMatrix(float nearPlane, float farPlane)
{
    return XMMatrixPerspectiveFovLH(XM_PI, 1.f, nearPlane, farPlane);
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
