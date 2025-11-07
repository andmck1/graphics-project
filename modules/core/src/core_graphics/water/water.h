#pragma once

#include <memory>

class Water
{
    public:
    std::unique_ptr<float> magnitude;
    std::unique_ptr<float> waveNumber;
    std::unique_ptr<float> angularVelocity;
};
