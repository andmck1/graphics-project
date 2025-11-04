#pragma once

#include "core_graphics/d3d/d3d.h"
#include "core_window/window.h"

class Config
{
    public:
    WindowConfig windowConfig = WindowConfig();
    D3DAppConfig d3dAppConfig = D3DAppConfig();

    bool initWindowConfig();
    bool initD3DAppConfig(Window *window);
};
