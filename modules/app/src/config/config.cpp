#include "config.h"

bool Config::initWindowConfig()
{
    this->windowConfig.height = 720;
    this->windowConfig.width = 1280;
    this->windowConfig.windowName = "Graphics Project";

    return true;
}

bool Config::initD3DAppConfig(Window *window)
{
    this->d3dAppConfig.window = window; 
    return true;
}
