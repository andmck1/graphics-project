#include "window.h"

#include <algorithm>
#include <string>

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_impl_win32.h>

const float gMOUSE_MAX_DELTA = 100.f;

Window::Window(WindowSpec spec)
{
    this->spec = spec;
    init();
}

bool Window::init()
{
    spdlog::debug("Initialising window");

    ImGui_ImplWin32_EnableDpiAwareness();
    if (!this->createWindow())
        return false;
    SetPropW(this->hwnd, L"WINDOWCLASS", (HANDLE) this);
    ShowWindow(this->hwnd, SW_SHOW);

    return true;
}

bool Window::createWindow()
{
    wc.lpfnWndProc = Window::onEvent;
    wc.hInstance = this->hInstance;
    wc.lpszClassName = this->spec.windowName.c_str();
    RegisterClass(&wc);
    this->msg.message = ~WM_QUIT;

    mainScale = ImGui_ImplWin32_GetDpiScaleForMonitor(
        MonitorFromPoint(POINT{0, 0}, MONITOR_DEFAULTTOPRIMARY)
    );
    RECT rect = {
        0, 0, (int) (mainScale * this->spec.width),
        (int) (mainScale * this->spec.height)
    };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    this->hwnd = CreateWindowEx(
        0, wc.lpszClassName, this->spec.windowName.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, this->hInstance, nullptr
    );

    if (!(this->hwnd))
        return false;
    return true;
}

void Window::destroyWindow()
{
    spdlog::debug("Destroying window");
    DestroyWindow(this->hwnd);
    UnregisterClassA(this->spec.windowName.c_str(), this->hInstance);
}

bool Window::exit()
{
    spdlog::debug("Closing window");
    this->unsetMouse();
    PostQuitMessage(0);
    return false;
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
        this->updateMouse();
        return true;
    }
    this->destroyWindow();
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

void Window::updateMouse()
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(this->hwnd, &cursorPos);

    RECT rect;
    GetClientRect(hwnd, &rect);
    int centreX = (rect.right - rect.left) / 2;
    int centreY = (rect.bottom - rect.top) / 2;

    float currX = static_cast<float>(cursorPos.x - centreX);
    float currY = static_cast<float>(cursorPos.y - centreY);

    this->mouse.dx = std::clamp(
        currX - this->mouse.oldX, -gMOUSE_MAX_DELTA, gMOUSE_MAX_DELTA
    );
    this->mouse.dy = std::clamp(
        currY - this->mouse.oldY, -gMOUSE_MAX_DELTA, gMOUSE_MAX_DELTA
    );

    this->mouse.oldX = currX;
    this->mouse.oldY = currY;
}

void Window::centreMouse()
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    int centreX = (rect.right - rect.left) / 2;
    int centreY = (rect.bottom - rect.top) / 2;
    POINT centrePoint = {centreX, centreY};
    ClientToScreen(hwnd, &centrePoint);
    SetCursorPos(centrePoint.x, centrePoint.y);
    this->mouse.oldX = 0.f;
    this->mouse.oldY = 0.f;
    this->mouse.dx = 0.f;
    this->mouse.dy = 0.f;
}

void Window::unsetMouse()
{
    this->mouse.isAvailable = false;
    ClipCursor(NULL);
    ShowCursor(TRUE);
}

void Window::setMouseVisible(bool isVisible)
{
    Input::Mouse::VisibleState mouseVisibleState = this->mouse.isVisible();

    switch (mouseVisibleState)
    {
    case Input::Mouse::VisibleState::VISIBLE:
        if (!isVisible)
        {
            ShowCursor(FALSE);
        }
        break;
    case Input::Mouse::VisibleState::NOT_VISIBLE:
        if (isVisible)
        {
            ShowCursor(TRUE);
        }
        break;
    default:
        return;
    }
}

bool Window::setFocusEvent()
{
    spdlog::info("Focus set");
    this->mouse.isAvailable = true;
    return 0;
}

bool Window::unsetFocusEvent()
{
    spdlog::info("Focus unset");
    this->unsetMouse();
    return 0;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
);
LRESULT CALLBACK
Window::onEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window *window = reinterpret_cast<Window *>(GetPropW(hwnd, L"WINDOWCLASS"));

    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SETFOCUS:
        return window->setFocusEvent();
    case WM_KILLFOCUS:
        return window->unsetFocusEvent();
    case WM_QUIT:
    case WM_CLOSE:
        return window->exit();
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
