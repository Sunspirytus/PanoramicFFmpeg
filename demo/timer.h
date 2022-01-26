#pragma once

#include <chrono>

class Timer
{
public:
    Timer();
    ~Timer();

    void SleepForFps();
    void SetFpsLimit(float fps);

private:
    float m_FpsLimit;
    std::chrono::system_clock::time_point m_BeginTime;
    std::chrono::system_clock::time_point m_EndTime;
    std::chrono::system_clock::duration m_InvFpsLimit;
};
