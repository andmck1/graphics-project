#pragma once

#include <string>
#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct WindowSpec
{
    int height;
    int width;
    std::string windowName;
};

struct WindowEvent
{
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
};

enum class Key
{
    ESC
};

class Window
{
    public:
    Window(WindowSpec spec);

    WindowSpec spec;
    HWND hwnd = nullptr;
    HINSTANCE hInstance;

    bool frame();
    bool exit();

    private:
    bool init();
    static MSG msg;
    static LRESULT CALLBACK
    WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
