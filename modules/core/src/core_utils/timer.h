#pragma once
#include <windows.h>

// (Robertson, n.d.)
// (Hughes, n.d.)
// (Falconer, n.d.)
class Timer
{
    public:
        Timer();
        
        INT64 tickFrequency = 1;
        float fps = 0.f;
        float deltaTime = 1.f;
        float totalTime = 0.f;

        void start();
        void frame();
    private:
        INT64 startTime = 0;
        INT64 previousCurrentTime = 0;
        INT64 timeSinceFrame = 0;
        int frameCount = 0;
        float elapsedTime = 0.f;
};
