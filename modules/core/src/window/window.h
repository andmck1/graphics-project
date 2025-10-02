#pragma once

#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct WindowSpec
{
    int height;
    int width;
    HWND hwnd = nullptr;
    HINSTANCE hInstance;
};

class Window
{
  public:
    Window(WindowSpec spec);
    HWND GetHWND();
    void Run();

  private:
    WindowSpec spec;
    bool InitWindow();
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow);
};
