#include "input.h"

Input::Mouse::VisibleState Input::Mouse::isVisible()
{
    CURSORINFO cursorInfo = {0};
    cursorInfo.cbSize = sizeof(cursorInfo);

    if (GetCursorInfo(&cursorInfo))
    {
        if (cursorInfo.flags == CURSOR_SHOWING)
        {
            return Input::Mouse::VisibleState::VISIBLE;
        }
        return Input::Mouse::VisibleState::NOT_VISIBLE;
    }
    return Input::Mouse::VisibleState::NOT_AVAILABLE;

}
