#pragma once

#include <string>
#include <unordered_map>
#include <windows.h>

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

namespace Input
{
    enum class Key
    {
        Escape,
        W,
        A,
        S,
        D,
        Q,
        E
    };

    enum class KeyState
    {
        PRESSED,
        RELEASED,
        NOT_IMPLEMENTED
    };

    static std::unordered_map<Key, int> keyToVKeyMapping = {
        {Key::W, 0x57}, {Key::A, 0x41}, {Key::S, 0x53},          {Key::D, 0x44},
        {Key::Q, 0x51}, {Key::E, 0x45}, {Key::Escape, VK_ESCAPE}
    };
} // namespace Input

class Window
{
    public:
    Window(WindowSpec spec);

    WindowSpec spec;
    HWND hwnd = nullptr;
    HINSTANCE hInstance;

    bool frame();
    Input::KeyState getKeyState(Input::Key key);
    static bool exit();

    private:
    bool init();
    MSG msg = {};
    static LRESULT CALLBACK
    WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
