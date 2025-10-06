#include "window/window.h"
#include "spdlog/spdlog.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting app");

    WindowSpec windowSpec;
    windowSpec.height = 600;
    windowSpec.width = 800;
    windowSpec.windowName = "Graphics Project";
    Window window = Window(windowSpec);
    while (window.Frame())
    {
    }

    return 0;
}
