#include "app.h"

App::App(AppState *state)
{
    this->state = state;
    this->state->timer = &this->timer;
}
