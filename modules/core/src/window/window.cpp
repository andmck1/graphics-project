#include "window.h"

Window::Window(WindowSpec spec)
{
    this->spec = spec;
}


HWND Window::GetHWND()
{
    return this->spec.hwnd;
}

bool Window::InitWindow()
{

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
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

int WINAPI Window::WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    this->spec.hInstance = hInstance;
    this->InitWindow();
    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}
