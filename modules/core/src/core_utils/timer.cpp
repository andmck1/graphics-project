#include "timer.h"
#include <profileapi.h>

// (Robertson, n.d.)
// (Hughes, n.d.)
// (Falconer, n.d.)
Timer::Timer()
{
    QueryPerformanceFrequency((LARGE_INTEGER *) &this->tickFrequency);
}

void Timer::start()
{
    QueryPerformanceCounter((LARGE_INTEGER *) &this->startTime);
    this->previousCurrentTime = this->startTime;
}

void Timer::frame()
{
    INT64 currentTime;

    this->frameCount++;
    QueryPerformanceCounter((LARGE_INTEGER *) &currentTime);

    this->timeSinceFrame = currentTime - this->previousCurrentTime;
    this->deltaTime = ((float) timeSinceFrame) / this->tickFrequency;
    this->elapsedTime += this->deltaTime;

    if (this->elapsedTime >= 1.f)
    {
        this->fps = this->frameCount / this->elapsedTime;
        this->elapsedTime = 0.f;
        this->frameCount = 0;
    }

    this->previousCurrentTime = currentTime;
    this->totalTime += deltaTime;
}
