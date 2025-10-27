#include <unordered_map>
#include <windows.h>

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
        P,
        LMB,
        RMB
    };

    enum class KeyState
    {
        PRESSED,
        RELEASED,
        NOT_IMPLEMENTED
    };

    static std::unordered_map<Key, int> keyToVKeyMapping = {
        {Key::W, 0x57},        {Key::A, 0x41},           {Key::S, 0x53},
        {Key::D, 0x44},        {Key::Q, 0x51},           {Key::E, 0x45},
        {Key::P, 0x50},        {Key::Escape, VK_ESCAPE}, {Key::LMB, VK_LBUTTON},
        {Key::RMB, VK_RBUTTON}
    };

    struct Mouse
    {
        float oldX = 0.f;
        float oldY = 0.f;
        float dx = 0.f;
        float dy = 0.f;
        bool isMovable = false;
    };
} // namespace Input
