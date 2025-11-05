#include "app.h"

App::App(AppState *state)
{
    this->state = state;
    this->state->appInput->deltaTime = &this->timer.deltaTime;
}
