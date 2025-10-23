#include "spdlog/spdlog.h"
#include <string>
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
    this->destroyWindow();
    return false;
}

void Window::destroyWindow()
{
    spdlog::debug("Destroying window");
    DestroyWindow(this->hwnd);
    UnregisterClassA(this->spec.windowName.c_str(), this->hInstance);
}

bool Window::init()
{
    spdlog::debug("Initialising window");

    WNDCLASS wc = {};
    wc.lpfnWndProc = Window::onEvent;
    wc.hInstance = this->hInstance;
    wc.lpszClassName = "DX11WindowClass";
    RegisterClass(&wc);
    this->msg.message = ~WM_QUIT;

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

    SetCursorPos(0, 0);
    this->mouse.x = 0.f;
    this->mouse.y = 0.f;

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

Input::MouseState Window::getMouseState()
{
    if (this->mouse.dx > this->mouse.jitterThreshold |
        this->mouse.dy > this->mouse.jitterThreshold |
        this->mouse.dx < this->mouse.jitterThreshold |
        this->mouse.dx < this->mouse.jitterThreshold)
    {
        return Input::MouseState::MOVED;
    }
    return Input::MouseState::NOT_MOVED;
}

void Window::updateMouse()
{
    POINT cursorPos;
    ScreenToClient(this->hwnd, &cursorPos);
    GetCursorPos(&cursorPos);

    float newX = (((float) cursorPos.x) / this->spec.width - this->spec.width);
    float newY =
        (((float) cursorPos.y) / this->spec.height - this->spec.height);
    this->mouse.dx = this->mouse.x - newX;
    this->mouse.dy = this->mouse.y - newY;
    this->mouse.x = newX;
    this->mouse.y = newY;
}

void Window::resetMouse()
{
    spdlog::info("Mouse reset");
    RECT rect;
    GetClientRect(hwnd, &rect);
    POINT ul = {rect.left, rect.top};
    POINT lr = {rect.right, rect.bottom};
    ClientToScreen(hwnd, &ul);
    ClientToScreen(hwnd, &lr);
    RECT clipRect = {ul.x, ul.y, lr.x, lr.y};
    ClipCursor(&clipRect);
    this->mouse.isMovable = true;
}

void Window::unsetMouse()
{
    spdlog::info("MOuse unset");
    ClipCursor(NULL);
    this->mouse.isMovable = false;
}

LRESULT CALLBACK
Window::onEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_QUIT:
    case WM_CLOSE:
        return Window::exit();
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
