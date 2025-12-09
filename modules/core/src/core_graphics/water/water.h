#pragma once

#include "core_graphics/actor/scene.h"

class Water : public SceneActor
{
    public:
    std::array<std::array<float, 3>, 2> direction;
    std::array<float, 2> magnitude;
    std::array<float, 2> waveNumber;
    std::array<float, 2> angularVelocity;
    std::array<float, 2> steepness;
};
