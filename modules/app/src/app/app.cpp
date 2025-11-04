#include "app.h"

const float gCAMERA_MOVE_SPEED = 0.01f;
const float gCAMERA_ROTATE_SENSITIVITY = .001f;


App::App(AppConfig *config)
{
    this->config = config;
}
