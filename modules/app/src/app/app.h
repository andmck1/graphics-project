#pragma once

#include <core_window/window.h>
#include <core_utils/timer.h>

#include "../input/input.h"

struct AppState
{
    AppInput *appInput;
};

class App
{
    public:
    AppState *state;
    App(AppState *state);

    Timer timer = Timer();
};
