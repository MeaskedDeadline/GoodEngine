#include "Timer.h"

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <time.h>
#endif

namespace Good
{

    int64 Timer::NowNs()
    {
#if defined(GOOD_PLATFORM_WINDOWS)
        static LARGE_INTEGER freq = {};
        static bool init = false;
        if (!init) { QueryPerformanceFrequency(&freq); init = true; }
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return static_cast<int64>(now.QuadPart * 1'000'000'000LL / freq.QuadPart);
#else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return static_cast<int64>(ts.tv_sec) * 1'000'000'000LL + ts.tv_nsec;
#endif
    }

    Timer::Timer() { Reset(); }

    void Timer::Start()
    {
        if (!m_Running) { m_StartNs = NowNs(); m_Running = true; }
    }

    void Timer::Stop()
    {
        if (m_Running) { m_AccumNs += NowNs() - m_StartNs; m_Running = false; }
    }

    void Timer::Reset()
    {
        m_StartNs = m_AccumNs = 0;
        m_Running = false;
    }

    void Timer::Restart()
    {
        Reset();
        Start();
    }

    float64 Timer::ElapsedSeconds() const
    {
        int64 ns = m_AccumNs + (m_Running ? NowNs() - m_StartNs : 0);
        return static_cast<float64>(ns) * 1e-9;
    }

    float64 Timer::ElapsedMilliseconds() const { return ElapsedSeconds() * 1e3; }
    float64 Timer::ElapsedMicroseconds() const { return ElapsedSeconds() * 1e6; }

} // namespace Good