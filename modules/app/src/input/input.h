#pragma once

#include "core_window/window.h"
#include "../scene/scene.h"

struct AppInputState
{
    Window *window;
    Scene *scene;
    float cameraMoveSpeed;
    float cameraRotateSensitivity;
    Timer *timer;
};

class AppInput
{
    public:
    AppInput(AppInputState *state);
    void handleInputs();

    private:
    AppInputState *state;
};
