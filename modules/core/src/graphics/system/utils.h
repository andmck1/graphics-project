#pragma once

#include <windows.h>

template <typename Fn> bool TryDXCall(Fn &&fn, const char *errorMessage);
