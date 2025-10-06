#pragma once

#include <string>
#include <unordered_map>
#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct WindowSpec
{
    int height;
    int width;
    std::string windowName;
    HWND hwnd = nullptr;
    HINSTANCE hInstance;
};

struct WindowEvent
{
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
};

class WindowEventHandler
{
    public:
    bool Poll(const WindowEvent &event);
    static bool DestroyEvent(WPARAM wParam, LPARAM lParam);
    static bool CharacterInputEvent(WPARAM wParam, LPARAM lParam);
    static bool MouseMoveEvent(WPARAM wParam, LPARAM lParam);

    private:
    static inline std::unordered_map<UINT, bool (*)(WPARAM, LPARAM)> eventMap =
        {{WM_DESTROY, &DestroyEvent},
         {WM_MOUSEMOVE, &MouseMoveEvent},
         {WM_CHAR, &CharacterInputEvent}};
};

class Window
{
    public:
    Window(WindowSpec spec);
    bool Frame();
    WindowEventHandler getEventHandler();

    private:
    MSG msg = {};
    WindowSpec spec;
    WindowEventHandler eventHandler = WindowEventHandler();
    bool InitWindow();
    static LRESULT CALLBACK
    WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
