#pragma once

#include <core_window/window.h>
#include <core_utils/timer.h>

#include "../input/input.h"

struct AppConfig
{
    AppInput *appInput;
};

class App
{
    public:
    AppConfig *config;
    App(AppConfig *config);

    Timer timer = Timer();
};
