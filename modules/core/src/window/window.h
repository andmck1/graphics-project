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
        E,
        LMB,
        RMB
    };

    enum class KeyState
    {
        PRESSED,
        RELEASED,
        NOT_IMPLEMENTED
    };

    enum class MouseState
    {
        MOVED,
        NOT_MOVED
    };

    static std::unordered_map<Key, int> keyToVKeyMapping = {
        {Key::W, 0x57},           {Key::A, 0x41},
        {Key::S, 0x53},           {Key::D, 0x44},
        {Key::Q, 0x51},           {Key::E, 0x45},
        {Key::Escape, VK_ESCAPE}, {Key::LMB, VK_LBUTTON},
        {Key::RMB, VK_RBUTTON}
    };

    struct Mouse
    {
        float x = 0.f;
        float y = 0.f;
        float dx = 0.f;
        float dy = 0.f;
        float jitterThreshold = 0.00001f;
        bool isMovable = false;
    };
} // namespace Input

class Window
{
    public:
    Window(WindowSpec spec);

    WindowSpec spec;
    HWND hwnd = nullptr;
    HINSTANCE hInstance;
    Input::Mouse mouse = Input::Mouse();

    bool frame();
    Input::KeyState getKeyState(Input::Key key);
    Input::MouseState getMouseState();
    static bool exit();
    void destroyWindow();

    private:
    MSG msg = {};

    static LRESULT CALLBACK
    onEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool init();
    void updateMouse();
    void resetMouse();
    void unsetMouse();
};
