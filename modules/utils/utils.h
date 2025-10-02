#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <string>
#include <windows.h>
#include <DirectXMath.h>

using namespace std::chrono;
using namespace DirectX;

constexpr float TWO_PI = 6.28318530718f;

template <typename Fn> bool TryDXCall(Fn &&fn, const char *errorMessage)
{
    if (FAILED(fn))
    {
        OutputDebugStringA(errorMessage);
        return false;
    }
    return true;
}

float CalculateFPS(int n_frames, double n_seconds);
float CheckAndMeasureFPS(
    float fps, int *frameCountPtr, time_point<steady_clock> *fpsStartTimePtr,
    duration<double> *durationSinceFPSStartTimePtr
);
std::wstring FPSAsString(float fps);
void PrintMatrix(const XMMATRIX &matrix, const char *name = "Matrix");

#endif
