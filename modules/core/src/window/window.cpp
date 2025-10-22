#include "spdlog/spdlog.h"
#include "window.h"

Window::Window(WindowSpec spec)
{
    this->spec = spec;
    init();
}

bool Window::frame()
{
    if (this->msg.message != WM_QUIT)
    {
        if (PeekMessage(&this->msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&this->msg);
            DispatchMessage(&this->msg);
        }
        return true;
    }
    else
    {
        this->exit();
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
    spdlog::debug("Closing window");
    PostQuitMessage(0);
    return false;
}

Input::KeyState Window::getKeyState(Input::Key key)
{
    if (Input::keyToVKeyMapping.find(key) != Input::keyToVKeyMapping.end())
    {
        int vkey = Input::keyToVKeyMapping[key];
        SHORT keyState = GetAsyncKeyState(vkey);
        if (keyState & 0x8000) // MSB tells us if key is currently pressed
        {
            return Input::KeyState::PRESSED;
        }
        return Input::KeyState::RELEASED;
    }
    return Input::KeyState::NOT_IMPLEMENTED;
}

LRESULT CALLBACK
Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        Window::exit();
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
