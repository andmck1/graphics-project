#include "window/window.h"

int main()
{
    WindowSpec windowSpec;
    windowSpec.height = 600;
    windowSpec.width = 800;
    Window window = Window(windowSpec);
    window.Run();

    return 0;
}
