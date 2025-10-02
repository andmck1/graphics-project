#include "app.h"
#include "../input/input.h"
#include <memory>

AppState &AppState::Get()
{
    static AppState instance;
    return instance;
}

bool AppState::Init(HINSTANCE hInstance, int width, int height)
{
    this->InitWindow(hInstance, width, height);
    this->InitInputDevices();
    return true;
}

bool AppState::InitInputDevices()
{
    RAWINPUTDEVICE rid{};
    rid.usUsagePage = 0x01;         // generic desktop controls
    rid.usUsage = 0x02;             // mouse
    rid.dwFlags = RIDEV_INPUTSINK   // receive even when inactive
                  | RIDEV_NOLEGACY; // stop WM_MOUSEMOVE / WM_LBUTTON / ...
    rid.hwndTarget = this->hwnd; 
    if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
    {
        MessageBox(
            hwnd, "RegisterRawInputDevices failed!", "Error", MB_ICONERROR
        );
        return false;
    }
    return true;
}

bool AppState::InitWindow(HINSTANCE hInstance, int width, int height)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "DX11WindowClass";

    RegisterClass(&wc);

    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND &hwnd = AppState::Get().hwnd;

    hwnd = CreateWindowExA(
        0, wc.lpszClassName, "DownTauntStudios", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd)
        return false;

    ShowWindow(hwnd, SW_SHOW);
    return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_INPUT:
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
            const float dx = static_cast<float>(raw->data.mouse.lLastX);
            const float dy = static_cast<float>(raw->data.mouse.lLastY);
            inputEvent(dx, dy); // ← your existing handler
        }
        return 0;
    }
    case WM_CHAR:
        inputEvent(static_cast<int>(wParam));
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
