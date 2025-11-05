#pragma once

#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class Camera
{
    public:
    Camera();
    XMMATRIX viewMatrix;
    XMVECTOR eyePosition = XMVectorSet(0.f, 0.f, -10.0f, 1.0f);
    // TODO - convert these to a single XMVECTOR
    float yaw = 0.0f;
    float pitch = 0.0f;

    XMMATRIX getCameraRotation();
    void move(float dx, float dy, float dz);
    void rotate(float dYaw, float dPitch);
    void update();
};
