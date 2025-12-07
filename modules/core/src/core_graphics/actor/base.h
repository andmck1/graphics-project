#pragma once

#include <array>
#include <directxmath.h>

using namespace DirectX;

constexpr float g_ROTATION_LIMIT = XM_2PI - 0.01f;

class BaseActor
{
    public:
    BaseActor();
    virtual ~BaseActor() = default;

    std::array<float, 3> worldPosition = {0.f, 0.f, 0.f};
    std::array<float, 3> rotation = {0.f, 0.f, 0.f};
    std::array<float, 3> scaling = {0.f, 0.f, 0.f};
    std::array<float, 3> forwardVector = {1.f, 0.f, 0.f};

    std::array<float, 3> previousWorldPosition = {0.f, 0.f, 0.f};
    std::array<float, 3> previousRotation = {0.f, 0.f, 0.f};
    std::array<float, 3> previousScaling = {0.f, 0.f, 0.f};
    std::array<float, 3> previousForwardVector = {1.f, 0.f, 0.f};

    XMMATRIX getWorldMatrix();
    XMMATRIX getTranslationMatrix();
    XMMATRIX getRotationMatrix();
    XMMATRIX getScalingMatrix();

    void updatePreviousWorldPosition();
    void updatePreviousRotation();
    void updatePreviousScaling();
    void updatePreviousForwardVector();

    void
    moveTo(float xWorldPosition, float yWorldPosition, float zWorldPosition);
    void moveBy(float xOffset, float yOffset, float zOffset);
    void
    moveRelativeToForwardVectorBy(float xOffset, float yOffset, float zOffset);

    void rotateTo(float pitch, float yaw, float roll);
    void rotateBy(float pitchOffset, float yawOffset, float rollOffset);

    void scaleTo(float xFactor, float yFactor, float zFactor);
    void scaleBy(float xFactorOffset, float yFactorOffset, float zFactorOffset);

    virtual void update() = 0;

    protected:
    void clampRotation();
    void clampScaling();
};
