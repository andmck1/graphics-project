#include "main.h"
#include "app/app.h"
#include "graphics/graphics.h"
#include "game/game.h"
#include "utils/utils.h"
#include <chrono>

using namespace std::chrono;


bool RenderLoop()
{
    Renderer &renderer = Renderer::Get();
    GameState &gameState = GameState::Get();

    float clearColour[4] = {0.0, 0.0, 0.0, 1.0};
    int frameCount = 0;
    time_point<steady_clock> fpsStartTime = steady_clock::now();
    time_point<steady_clock> currentTime = steady_clock::now();
    float fps;
    std::wstring fpsText = L"";
    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        frameCount++;

        duration<double> durationSinceEpoch = steady_clock::now() - currentTime;
        duration<double> durationSinceFPSStartTime =
            steady_clock::now() - fpsStartTime;
        gameState.setDeltaTime(durationSinceEpoch.count());
        currentTime = steady_clock::now();

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            fps = CheckAndMeasureFPS(
                fps, &frameCount, &fpsStartTime, &durationSinceFPSStartTime
            );
            fpsText = FPSAsString(fps);

            if (!renderer.RenderFrame(clearColour, fpsText))
            {
                return false;
            }
        }
    }

    return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    const int width = 800;
    const int height = 600;
    AppState &appState = AppState::Get();

    if (!appState.Init(hInstance, width, height))
        return -1;
    if (!InitD3D(width, height))
        return -1;
    if (!InitD2DResources())
        return -1;

    if (!RenderLoop())
    {
        return -1;
    }

    return 0;
}
