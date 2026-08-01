#include "Time.h"

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <time.h>
#endif

namespace Good
{

float32 Time::s_DeltaTime      = 0.0f;
float32 Time::s_UnscaledDelta  = 0.0f;
float64 Time::s_Elapsed        = 0.0;
float64 Time::s_LastTime       = 0.0;
uint64  Time::s_FrameCount     = 0;
float32 Time::s_FPS            = 0.0f;
float32 Time::s_TimeScale      = 1.0f;
float32 Time::s_FixedDeltaTime = 1.0f / 60.0f;
float64 Time::s_FPSTimer       = 0.0;
uint32  Time::s_FPSFrameCount  = 0;

static float64 PlatformSeconds()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    static LARGE_INTEGER freq = {};
    static bool init = false;
    if (!init) { QueryPerformanceFrequency(&freq); init = true; }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return static_cast<float64>(now.QuadPart) / static_cast<float64>(freq.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<float64>(ts.tv_sec) + static_cast<float64>(ts.tv_nsec) * 1e-9;
#endif
}

void Time::Initialize()
{
    s_LastTime = PlatformSeconds();
    s_Elapsed = 0.0; s_DeltaTime = 0.0f;
    s_UnscaledDelta = 0.0f; s_FrameCount = 0;
    s_FPS = 0.0f; s_FPSTimer = 0.0; s_FPSFrameCount = 0;
}

void Time::Tick()
{
    float64 now = PlatformSeconds();
    float64 raw = now - s_LastTime;
    s_LastTime  = now;
    if (raw > 0.25) raw = 0.25;

    s_UnscaledDelta  = static_cast<float32>(raw);
    s_DeltaTime      = s_UnscaledDelta * s_TimeScale;
    s_Elapsed       += raw;
    ++s_FrameCount;

    s_FPSTimer += raw;
    ++s_FPSFrameCount;
    if (s_FPSTimer >= 1.0)
    {
        s_FPS = static_cast<float32>(s_FPSFrameCount) / static_cast<float32>(s_FPSTimer);
        s_FPSTimer = 0.0; s_FPSFrameCount = 0;
    }
}

int64 Stopwatch::NowNs()
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
    return static_cast<int64>(ts.tv_sec) * 1'000'000'000LL + static_cast<int64>(ts.tv_nsec);
#endif
}

Stopwatch::Stopwatch() { Reset(); }
void Stopwatch::Start() { if (!m_Running) { m_StartNs = NowNs(); m_Running = true; } }
void Stopwatch::Stop()  { if (m_Running)  { m_AccumNs += NowNs() - m_StartNs; m_Running = false; } }
void Stopwatch::Reset() { m_StartNs = 0; m_AccumNs = 0; m_Running = false; }

float64 Stopwatch::ElapsedSeconds()      const { int64 ns = m_AccumNs + (m_Running ? NowNs() - m_StartNs : 0); return static_cast<float64>(ns) * 1e-9; }
float64 Stopwatch::ElapsedMilliseconds() const { return ElapsedSeconds() * 1000.0; }

} // namespace Good