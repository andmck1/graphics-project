#pragma once

#include <wrl.h>
#include <spdlog/spdlog.h>

template <typename Fn> bool TryDXCall(Fn &&fn, const char *errorMessage)
{
    if (FAILED(fn))
    {
        spdlog::error(errorMessage);
        return false;
    }
    return true;
}

