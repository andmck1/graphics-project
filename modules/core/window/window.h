#pragma once
#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class AppState
{
  public:
    static AppState &Get();
    bool Init(HINSTANCE hInstance, int width, int height);
    HWND hwnd = nullptr;

  private:
    AppState() = default;
    AppState(const AppState &) = delete;
    void operator=(const AppState &) = delete;

    bool InitInputDevices();
    bool InitWindow(HINSTANCE hInstance, int width, int height);
};
