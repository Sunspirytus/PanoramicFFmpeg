#include "timer.h"
#include <chrono>
#include <thread>

Timer::Timer()
    : m_FpsLimit(0.0f)
{
    m_BeginTime = std::chrono::system_clock::now();
    m_EndTime = std::chrono::system_clock::now();
}

Timer::~Timer()
{

}

void Timer::SetFpsLimit(float fps)
{
    m_FpsLimit = fps;
    m_InvFpsLimit = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>(1.0 / m_FpsLimit));
}

void Timer::SleepForFps()
{
    if (m_FpsLimit <= 0.0f) 
    {
        return;
    }

    m_EndTime = m_BeginTime + m_InvFpsLimit;
    std::this_thread::sleep_until(m_EndTime);
    m_BeginTime = m_EndTime;
}