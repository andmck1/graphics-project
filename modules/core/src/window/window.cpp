#include <memory>
#include "spdlog/spdlog.h"
#include "window.h"

std::unique_ptr<WindowEventHandler> g_windowEventHandler;

Window::Window(WindowSpec spec)
{
    spec = spec;
    InitWindow();
}

bool Window::Frame()
{
    if (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return 1;
    }
    return 0;
}

bool Window::InitWindow()
{
    spdlog::debug("Initialising window");

    WNDCLASS wc = {};
    wc.lpfnWndProc = Window::WndProc;
    wc.hInstance = this->spec.hInstance;
    wc.lpszClassName = "DX11WindowClass";

    RegisterClass(&wc);

    RECT rect = {0, 0, this->spec.width, this->spec.height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    this->spec.hwnd = CreateWindowExA(
        0, wc.lpszClassName, this->spec.windowName.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, this->spec.hInstance, nullptr
    );

    if (!(this->spec.hwnd))
        return false;

    ShowWindow(this->spec.hwnd, SW_SHOW);
    return true;
}

bool WindowEventHandler::DestroyEvent(WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 1;
}

bool WindowEventHandler::MouseMoveEvent(WPARAM wParam, LPARAM lParam)
{
    return 1;
}

bool WindowEventHandler::CharacterInputEvent(WPARAM wParam, LPARAM lParam)
{
    return 1;
}

bool WindowEventHandler::Poll(const WindowEvent &event)
{
    auto mapPair = eventMap.find(event.msg);
    if (mapPair != eventMap.end())
    {
        return mapPair->second(event.wParam, event.lParam);
    }
    return 0;
}

LRESULT CALLBACK
Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    const WindowEvent &event = WindowEvent{msg, wParam, lParam};
    if (g_windowEventHandler->Poll(event))
    {
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
