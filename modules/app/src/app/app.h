#pragma once

#include "../scene/scene.h"
#include "window/window.h"

class App
{
    public:
    App(Scene *scene, Window *window);
    void handleInputs();

    private:
    Scene *scene;
    Window *window;
};
