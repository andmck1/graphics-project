#pragma once

#include "core_graphics/actor/scene.h"
#include "core_graphics/d3d/shadowmap.h"
#include <directxmath.h>

using namespace DirectX;

class SpotLight : public SceneActor
{
    public:
    std::array<float, 3> directionVector;
    float coneAngle;
    std::array<float, 3> diffuseColour = {0.f, 0.f, 0.f};
    std::array<float, 3> specularColour = {0.f, 0.f, 0.f};
    float specularPower;
    std::array<float, 3> attenuationCoefficients = {0.f, 0.f, 0.f};
    bool isDiffuseEnabled;
    bool isSpecularEnabled;
    bool isAttenuationEnabled;

    ShadowMap shadowMap = ShadowMap();

    XMMATRIX calculateViewMatrix();
    XMMATRIX calculateProjectionMatrix(float nearPlane, float farPlane);
    XMMATRIX calculateOrthographicProjectionMatrix(
        float screenWidth, float screenHeight, float nearPlane, float farPlane
    );
};
