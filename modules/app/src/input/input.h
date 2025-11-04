#pragma once

#include "core_window/window.h"
#include "../scene/scene.h"

struct AppInputConfig
{
    Window *window;
    Scene *scene;
    float cameraMoveSpeed;
    float cameraRotateSensitivity;
};

class AppInput
{
    public:
    AppInput(AppInputConfig *config);
    void handleInputs(float deltaTime);

    private:
    AppInputConfig *config;
};
