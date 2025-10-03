#include <any>
#include <memory>
#include <iostream>
#include "window.h"

std::unique_ptr<IEventSystem> g_eventSystem;

Window::Window(WindowSpec spec)
{
    this->spec = spec;
}

void Window::Run()
{
    if (!this->InitWindow())
    {
        return;
    }

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

bool Window::InitWindow()
{
    g_eventSystem = std::make_unique<WindowsEvents>();
    WNDCLASS wc = {};
    wc.lpfnWndProc = Window::WndProc;
    wc.hInstance = this->spec.hInstance;
    wc.lpszClassName = "DX11WindowClass";

    RegisterClass(&wc);

    RECT rect = {0, 0, this->spec.width, this->spec.height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    this->spec.hwnd = CreateWindowExA(
        0, wc.lpszClassName, "DownTauntStudios", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, this->spec.hInstance, nullptr
    );

    if (!(this->spec.hwnd))
        return false;

    ShowWindow(this->spec.hwnd, SW_SHOW);
    return true;
}

int WindowsEvents::DestroyEvent(WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 1;
}

int WindowsEvents::InputEvent(WPARAM wParam, LPARAM lParam)
{
    UINT size = 0;
    // First call – get required buffer size
    GetRawInputData(
        reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size,
        sizeof(RAWINPUTHEADER)
    );

    if (size == 0)
        return 0; // should never happen

    // Stack buffer for small packets, heap for > sizeof(tmp)
    BYTE tmp[sizeof(RAWINPUT) * 2];
    std::unique_ptr<BYTE[]> heap;
    BYTE *buffer = size <= sizeof(tmp)
                       ? tmp
                       : (heap = std::make_unique<BYTE[]>(size)).get();

    // Second call – fetch the data
    if (GetRawInputData(
            reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size,
            sizeof(RAWINPUTHEADER)
        ) != size)
        return 0; // malformed packet

    auto *raw = reinterpret_cast<RAWINPUT *>(buffer);
    if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        // const float dx = static_cast<float>(raw->data.mouse.lLastX);
        // const float dy = static_cast<float>(raw->data.mouse.lLastY);
    }
    return 1;
}

int WindowsEvents::CharacterInputEvent(WPARAM wParam, LPARAM lParam)
{
    std::cout << "OIOIOI";
    return 1;
}

int WindowsEvents::Poll(const Event &event)
{
    const WindowsEventInputData &windowsEventData =
        std::any_cast<const WindowsEventInputData &>(event.data);
    auto mapPair = eventMap.find(windowsEventData.msg);
    if (mapPair != eventMap.end())
    {
        return mapPair->second(
            windowsEventData.wParam, windowsEventData.lParam
        );
    }
    return 0;
}

LRESULT CALLBACK
Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    const Event &event = Event{WindowsEventInputData{msg, wParam, lParam}};
    if (g_eventSystem->Poll(event))
    {
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
