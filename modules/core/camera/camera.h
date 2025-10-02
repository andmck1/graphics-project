#pragma once
#include <directxmath.h>

using namespace DirectX;

class Camera
{
  public:
    Camera();
    XMMATRIX getCameraRotation();
    void calculateViewMatrix();
    void move(XMVECTOR moveAmount);
    void rotate(float dYaw, float dPitch);

    XMMATRIX viewMatrix;

  private:
    XMVECTOR eyePosition = XMVectorSet(0.f, 0.f, -10.f, 1.0f);
    float yaw = 0.f;
    float pitch = 0.f;
};
