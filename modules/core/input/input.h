#pragma once

#include <unordered_map>

enum InputEnum
{
    cameraForward,
    cameraBack,
    cameraLeft,
    cameraRight,
    quit,
};

struct InputMapping
{
    std::unordered_map<int, InputEnum> action;
};

class Input
{
  public:
    static Input &Get();

    InputMapping mapping;

    Input();
};

void inputEvent(int keyCode);
void inputEvent(const double mouseDx, const double mouseDy);
