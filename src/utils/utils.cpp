#include "utils.h"
#include <sstream>

float CalculateFPS(int n_frames, double n_seconds)
{
    return (float) n_frames / (float) n_seconds;
}

float CheckAndMeasureFPS(
    float fps, int *frameCountPtr, time_point<steady_clock> *fpsStartTimePtr,
    duration<double> *durationSinceFPSStartTimePtr
)
{
    if (*frameCountPtr >= 50)
    {
        fps = CalculateFPS(
            *frameCountPtr,
            duration<double>(*durationSinceFPSStartTimePtr).count()
        );
        *frameCountPtr = 0;
        *fpsStartTimePtr = steady_clock::now();
    }
    return fps;
}

std::wstring FPSAsString(float fps)
{
    return L"FPS: " + std::to_wstring((int) fps);
}

void PrintMatrix(const XMMATRIX &matrix, const char *name)
{
    XMFLOAT4X4 m;
    XMStoreFloat4x4(&m, matrix);

    std::ostringstream oss;
    oss << name << ":\n";
    for (int i = 0; i < 4; ++i)
    {
        oss << "  ";
        for (int j = 0; j < 4; ++j)
        {
            oss << m.m[i][j] << "\t";
        }
        oss << "\n";
    }

    OutputDebugStringA(oss.str().c_str());
}
