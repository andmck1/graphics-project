#include "input.h"

AppInput::AppInput(AppInputState *state)
{
    this->state = state;
}

void AppInput::handleInputs()
{
    if (this->state->window->mouse.isAvailable)
    {

        // Quit
        if (this->state->window->getKeyState(Input::Key::Escape) ==
            Input::KeyState::PRESSED)
        {
            this->state->window->exit();
        }

        // Movement
        if (this->state->window->getKeyState(Input::Key::W) ==
            Input::KeyState::PRESSED)
        {
            this->state->scene->state->camera->moveRelativeToForwardVectorBy(
                0.f, 0.f,
                1.f * this->state->cameraMoveSpeed *
                    this->state->timer->deltaTime
            );
        }
        if (this->state->window->getKeyState(Input::Key::S) ==
            Input::KeyState::PRESSED)
        {
            this->state->scene->state->camera->moveRelativeToForwardVectorBy(
                0.f, 0.f,
                -1.f * this->state->cameraMoveSpeed *
                    this->state->timer->deltaTime
            );
        }
        if (this->state->window->getKeyState(Input::Key::A) ==
            Input::KeyState::PRESSED)
        {
            this->state->scene->state->camera->moveRelativeToForwardVectorBy(
                1.f * -this->state->cameraMoveSpeed *
                    this->state->timer->deltaTime,
                0.f, 0.f
            );
        }
        if (this->state->window->getKeyState(Input::Key::D) ==
            Input::KeyState::PRESSED)
        {
            this->state->scene->state->camera->moveRelativeToForwardVectorBy(
                1.f * this->state->cameraMoveSpeed *
                    this->state->timer->deltaTime,
                0.f, 0.f
            );
        }
        if (this->state->window->getKeyState(Input::Key::Q) ==
            Input::KeyState::PRESSED)
        {
            this->state->scene->state->camera->moveRelativeToForwardVectorBy(
                0.f,
                1.f * this->state->cameraMoveSpeed *
                    this->state->timer->deltaTime,
                0.f
            );
        }
        if (this->state->window->getKeyState(Input::Key::E) ==
            Input::KeyState::PRESSED)
        {
            this->state->scene->state->camera->moveRelativeToForwardVectorBy(
                0.f,
                -1.f * this->state->cameraMoveSpeed *
                    this->state->timer->deltaTime,
                0.f
            );
        }

        // Camera
        if (this->state->window->getKeyState(Input::Key::RMB) ==
            Input::KeyState::PRESSED)
        {
            this->state->window->setMouseVisible(false);
            this->state->scene->state->camera->rotateBy(
                this->state->window->mouse.dy *
                    this->state->cameraRotateSensitivity *
                    this->state->timer->deltaTime,
                this->state->window->mouse.dx *
                    this->state->cameraRotateSensitivity *
                    this->state->timer->deltaTime,
                0.f
            );
            this->state->window->centreMouse();
        }
        else if (this->state->window->getKeyState(Input::Key::RMB) ==
                 Input::KeyState::RELEASED)
        {
            this->state->window->setMouseVisible(true);
        }
    }
}
