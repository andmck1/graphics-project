#pragma once

#include <core_window/window.h>

#include "../scene/scene.h"

class App
{
    public:
    App(Scene *scene, Window *window);
    void handleInputs();

    private:
    Scene *scene;
    Window *window;
};
