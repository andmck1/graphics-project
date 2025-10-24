#include "app.h"

const float gCAMERA_MOVE_SPEED = 0.01f;
const float gCAMERA_ROTATE_SENSITIVITY = .1f;

App::App(Scene *scene, Window *window)
{
    this->scene = scene;
    this->window = window;
}

void App::handleInputs()
{
    if (this->window->getKeyState(Input::Key::Escape) ==
        Input::KeyState::PRESSED)
    {
        this->window->exit();
    }
    if (this->window->getKeyState(Input::Key::W) == Input::KeyState::PRESSED)
    {
        this->scene->camera->move(0.f, 0.f, 1.f * gCAMERA_MOVE_SPEED);
    }
    if (this->window->getKeyState(Input::Key::S) == Input::KeyState::PRESSED)
    {
        this->scene->camera->move(0.f, 0.f, -1.f * gCAMERA_MOVE_SPEED);
    }
    if (this->window->getKeyState(Input::Key::A) == Input::KeyState::PRESSED)
    {
        this->scene->camera->move(1.f * -gCAMERA_MOVE_SPEED, 0.f, 0.f);
    }
    if (this->window->getKeyState(Input::Key::D) == Input::KeyState::PRESSED)
    {
        this->scene->camera->move(1.f * gCAMERA_MOVE_SPEED, 0.f, 0.f);
    }
    if (this->window->getKeyState(Input::Key::Q) == Input::KeyState::PRESSED)
    {
        this->scene->camera->move(0.f, 1.f * gCAMERA_MOVE_SPEED, 0.f);
    }
    if (this->window->getKeyState(Input::Key::E) == Input::KeyState::PRESSED)
    {
        this->scene->camera->move(0.f, -1.f * gCAMERA_MOVE_SPEED, 0.f);
    }
    if ((this->window->getKeyState(Input::Key::RMB) == Input::KeyState::PRESSED
        ) &&
        this->window->mouse.isMovable)
    {
        this->scene->camera->rotate(
            this->window->mouse.dx * gCAMERA_ROTATE_SENSITIVITY,
            this->window->mouse.dy * gCAMERA_ROTATE_SENSITIVITY
        );
    }
}
