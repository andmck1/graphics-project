#pragma once

#include <string>
#include <windows.h>

#include "../input/input.h"

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

class Window
{
    public:
    Window(WindowSpec spec);
    ~Window();

    WindowSpec spec;
    HWND hwnd = nullptr;
    HINSTANCE hInstance;
    Input::Mouse mouse = Input::Mouse();
    float mainScale;

    bool createWindow();
    bool exit();
    void destroyWindow();
    bool frame();
    Input::KeyState getKeyState(Input::Key key);

    private:
    MSG msg = {};
    WNDCLASS wc = {};

    bool init();
    void updateMouse();
    void resetMouse();
    void unsetMouse();
    void centreMouse();

    static LRESULT CALLBACK
    onEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
