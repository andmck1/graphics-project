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

    WindowSpec spec;
    HWND hwnd = nullptr;
    HINSTANCE hInstance;
    Input::Mouse mouse = Input::Mouse();
    float mainScale;

    bool exit();
    bool frame();
    Input::KeyState getKeyState(Input::Key key);
    void setMouseVisible(bool isVisible);
    void centreMouse();

    private:
    MSG msg = {};
    WNDCLASS wc = {};

    bool init();
    bool createWindow();
    void destroyWindow();
    void updateMouse();
    void unsetMouse();
    bool setFocusEvent();
    bool unsetFocusEvent();

    static LRESULT CALLBACK
    onEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
