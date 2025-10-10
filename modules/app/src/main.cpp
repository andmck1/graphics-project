#include "spdlog/spdlog.h"

#include "window/window.h"
#include "graphics/system/d3d.h"
#include "game/manager.h"
#include "graphics/scene/default.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting app");

    WindowSpec windowSpec;
    windowSpec.height = 600;
    windowSpec.width = 800;
    windowSpec.windowName = "Graphics Project";
    Window window = Window(windowSpec);

    D3DApp graphicsApp = D3DApp();
    graphicsApp.Init(window.hwnd, windowSpec.width, windowSpec.height);

    DefaultScene scene = DefaultScene(
        graphicsApp.deviceManager.device,
        graphicsApp.deviceManager.deviceContext
    );
    GameManager gameManager = GameManager(
        graphicsApp.deviceManager.device,
        graphicsApp.deviceManager.deviceContext, scene
    );

    while (window.Frame())
    {
        scene.Render();
    }

    return 0;
}
