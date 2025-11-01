#pragma once

#include <core_window/window.h>
#include <core_utils/timer.h>

#include "../scene/scene.h"

class App
{
    public:
    App(Scene *scene, Window *window);

    Timer timer = Timer();

    void handleInputs();

    private:
    Scene *scene;
    Window *window;
};
