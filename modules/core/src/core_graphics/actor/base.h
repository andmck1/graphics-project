#pragma once

#include <array>
#include <directxmath.h>

using namespace DirectX;

constexpr float g_ROTATION_LIMIT = XM_PIDIV2 - 0.01f;

class BaseActor
{
    public:
    virtual ~BaseActor() = default;

    std::array<float, 3> worldPosition = {0.f, 0.f, 0.f};
    std::array<float, 3> rotation = {0.f, 0.f, 0.f};
    std::array<float, 3> scaling = {0.f, 0.f, 0.f};
    bool isDrawEnabled = true;

    XMMATRIX getWorldMatrix();
    XMMATRIX getTranslationMatrix();
    XMMATRIX getRotationMatrix();
    XMMATRIX getScalingMatrix();

    void
    moveTo(float xWorldPosition, float yWorldPosition, float zWorldPosition);
    void moveBy(float xOffset, float yOffset, float zOffset);

    void rotateTo(float pitch, float yaw, float roll);
    void rotateBy(float pitchOffset, float yawOffset, float rollOffset);

    void scaleTo(float xFactor, float yFactor, float zFactor);
    void scaleBy(float xFactorOffset, float yFactorOffset, float zFactorOffset);

    virtual void update() = 0;

    protected:
    void clampRotation();
    void clampScaling();
};
