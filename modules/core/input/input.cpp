#include "input.h"
#include "../game/game.h"
#include <windows.h>
#include <DirectXMath.h>

using namespace DirectX;
Input &Input::Get()
{
    static Input instance;
    return instance;
}

// TODO: Load from config file
Input::Input()
{
    this->mapping.action['w'] = InputEnum::cameraForward;
    this->mapping.action['a'] = InputEnum::cameraLeft;
    this->mapping.action['s'] = InputEnum::cameraBack;
    this->mapping.action['d'] = InputEnum::cameraRight;
    this->mapping.action[27] = InputEnum::quit;
}

void inputEvent(int keyCode)
{
    GameState &gameState = GameState::Get();
    std::unordered_map<int, InputEnum>::const_iterator inputAction =
        Input::Get().mapping.action.find(keyCode);

    if (inputAction != Input::Get().mapping.action.end())
    {
        switch (inputAction->second)
        {
        case InputEnum::cameraForward:
            gameState.moveCameraAndUpdate(XMVectorSet(0.f, 0.f, .1f, 0.f));
            break;
        case InputEnum::cameraBack:
            gameState.moveCameraAndUpdate(XMVectorSet(0.f, 0.f, -.1f, 0.f));
            break;
        case InputEnum::cameraLeft:
            gameState.moveCameraAndUpdate(XMVectorSet(-0.1f, 0.f, 0.f, 0.f));
            break;
        case InputEnum::cameraRight:
            gameState.moveCameraAndUpdate(XMVectorSet(.1f, 0.f, 0.f, 0.f));
            break;
        case InputEnum::quit:
            PostQuitMessage(0);
        default:
            break;
        }
    }
}

void inputEvent(const double mouseDx, const double mouseDy)
{
    GameState &gameState = GameState::Get();
    gameState.rotateCameraAndUpdate(mouseDx, mouseDy);
}
