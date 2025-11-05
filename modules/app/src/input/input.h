#pragma once

#include "core_window/window.h"
#include "../scene/scene.h"

struct AppInputState
{
    Window *window;
    Scene *scene;
    float cameraMoveSpeed;
    float cameraRotateSensitivity;
};

class AppInput
{
    public:
    AppInput(AppInputState *state);
    void handleInputs();
    float *deltaTime;

    private:
    AppInputState *state;
};
