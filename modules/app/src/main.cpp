#include "scene/scene.h"
#include "spdlog/spdlog.h"

#include "window/window.h"
#include "graphics/d3d.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting app");
    
    // TODO - move this to config file
    WindowSpec windowSpec;
    windowSpec.height = 600;
    windowSpec.width = 800;
    windowSpec.windowName = "Graphics Project";
    Window window = Window(windowSpec);

    D3D graphicsManager = D3D(&window);
    graphicsManager.init();
    
    TriangleMesh mesh = TriangleMesh(&graphicsManager);
    Camera camera = Camera();
    Scene scene = Scene(&graphicsManager);
    scene.mesh = &mesh;
    scene.camera = &camera;
    scene.meshWorldMatrix = XMMatrixIdentity();

    while (window.frame())
    {
        scene.render();
    }

    return 0;
}
