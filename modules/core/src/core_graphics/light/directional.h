#pragma once

#include "core_graphics/actor/scene.h"
#include <array>
#include <directxmath.h>

using namespace DirectX;

class DirectionalLight : public SceneActor
{
    public:
    std::array<float, 3> diffuseColour = {0.f, 0.f, 0.f};
    std::array<float, 3> specularColour = {0.f, 0.f, 0.f};
    float specularPower;
    std::array<float, 3> directionVector = {0.f, 0.f, 0.f};
    bool isDiffuseEnabled;
    bool isSpecularEnabled;
    ShadowMap shadowMap = ShadowMap();
    float orthodistance = 100.f;

    XMMATRIX calculateViewMatrix();
    XMMATRIX calculateOrthographicProjectionMatrix(
        float screenWidth, float screenHeight, float nearPlane, float farPlane
    );
};
