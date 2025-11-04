#include "input.h"

AppInput::AppInput(AppInputConfig *config)
{
    this->config = config;
}

void AppInput::handleInputs(float deltaTime)
{
    if (this->config->window->mouse.isAvailable)
    {

        // Quit
        if (this->config->window->getKeyState(Input::Key::Escape) ==
            Input::KeyState::PRESSED)
        {
            this->config->window->exit();
        }

        // Movement
        if (this->config->window->getKeyState(Input::Key::W) ==
            Input::KeyState::PRESSED)
        {
            this->config->scene->camera->move(
                0.f, 0.f, 1.f * this->config->cameraMoveSpeed * deltaTime
            );
        }
        if (this->config->window->getKeyState(Input::Key::S) ==
            Input::KeyState::PRESSED)
        {
            this->config->scene->camera->move(
                0.f, 0.f, -1.f * this->config->cameraMoveSpeed * deltaTime
            );
        }
        if (this->config->window->getKeyState(Input::Key::A) ==
            Input::KeyState::PRESSED)
        {
            this->config->scene->camera->move(
                1.f * -this->config->cameraMoveSpeed * deltaTime, 0.f, 0.f
            );
        }
        if (this->config->window->getKeyState(Input::Key::D) ==
            Input::KeyState::PRESSED)
        {
            this->config->scene->camera->move(
                1.f * this->config->cameraMoveSpeed * deltaTime, 0.f, 0.f
            );
        }
        if (this->config->window->getKeyState(Input::Key::Q) ==
            Input::KeyState::PRESSED)
        {
            this->config->scene->camera->move(
                0.f, 1.f * this->config->cameraMoveSpeed * deltaTime, 0.f
            );
        }
        if (this->config->window->getKeyState(Input::Key::E) ==
            Input::KeyState::PRESSED)
        {
            this->config->scene->camera->move(
                0.f, -1.f * this->config->cameraMoveSpeed * deltaTime, 0.f
            );
        }

        // Camera
        if (this->config->window->getKeyState(Input::Key::RMB) ==
            Input::KeyState::PRESSED)
        {
            this->config->window->setMouseVisible(false);
            this->config->scene->camera->rotate(
                this->config->window->mouse.dx *
                    this->config->cameraRotateSensitivity * deltaTime,
                this->config->window->mouse.dy *
                    this->config->cameraRotateSensitivity * deltaTime
            );
            this->config->window->centreMouse();
        }
        else if (this->config->window->getKeyState(Input::Key::RMB) ==
                 Input::KeyState::RELEASED)
        {
            this->config->window->setMouseVisible(true);
        }
    }
}
