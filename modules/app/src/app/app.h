#pragma once

#include <core_window/window.h>
#include <core_utils/timer.h>

struct AppState
{
    Timer *timer;
};

class App
{
    public:
    AppState *state;
    App(AppState *state);

    Timer timer = Timer();
};
