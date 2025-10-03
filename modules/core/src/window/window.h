#pragma once

#include <any>
#include <unordered_map>
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

struct Event
{
    std::any data;
};

class IEventSystem
{
  public:
    virtual ~IEventSystem() = default;
    virtual int Poll(const Event &event) = 0;
};

struct WindowsEventInputData
{
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
};

class WindowsEvents : public IEventSystem
{
  public:
    int Poll(const Event &event) override;
    static int DestroyEvent(WPARAM wParam, LPARAM lParam);
    static int InputEvent(WPARAM wParam, LPARAM lParam);
    static int CharacterInputEvent(WPARAM wParam, LPARAM lParam);

  private:
    static inline std::unordered_map<UINT, int (*)(WPARAM, LPARAM)> eventMap = {
        {WM_DESTROY, &DestroyEvent},
        {WM_INPUT, &InputEvent},
        {WM_CHAR, &CharacterInputEvent}
    };
};

class Window
{
  public:
    Window(WindowSpec spec);
    void Run();

  private:
    WindowSpec spec;
    WindowsEvents eventSystem = WindowsEvents();
    bool InitWindow();
    static LRESULT CALLBACK
    WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
