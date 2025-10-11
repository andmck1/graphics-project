#include "spdlog/spdlog.h"
#include "window.h"



Window::Window(WindowSpec spec)
{
    this->spec = spec;
    init();
}

bool Window::frame()
{
    if (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }
    return false;
}

bool Window::init()
{
    spdlog::debug("Initialising window");

    WNDCLASS wc = {};
    wc.lpfnWndProc = Window::WndProc;
    wc.hInstance = this->hInstance;
    wc.lpszClassName = "DX11WindowClass";

    RegisterClass(&wc);

    RECT rect = {0, 0, this->spec.width, this->spec.height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    this->hwnd = CreateWindowExA(
        0, wc.lpszClassName, this->spec.windowName.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, this->hInstance, nullptr
    );

    if (!(this->hwnd))
        return false;

    ShowWindow(this->hwnd, SW_SHOW);
    return true;
}

bool Window::exit()
{
    PostQuitMessage(0);
    return false;
}

LRESULT CALLBACK
Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
