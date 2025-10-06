#include "graphics/system/utils.h"

template <typename Fn> bool TryDXCall(Fn &&fn, const char *errorMessage)
{
    if (FAILED(fn))
    {
        OutputDebugStringA(errorMessage);
        return false;
    }
    return true;
}
