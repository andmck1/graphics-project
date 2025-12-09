#pragma once

#include <wrl.h>
#include <spdlog/spdlog.h>

// Falloff function from copilot, works well
template <typename Fn> bool TryDXCall(Fn &&fn, const char *errorMessage)
{
    if (FAILED(fn))
    {
        spdlog::error(errorMessage);
        return false;
    }
    return true;
}

