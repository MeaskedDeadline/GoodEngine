#include "Debug/Profiler.h"
#include "Core/Log.h"
#include <cstdio>

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <time.h>
#endif

namespace Good
{

int64 Profiler::NowNs()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    static LARGE_INTEGER freq = {};
    static bool init = false;
    if (!init) { QueryPerformanceFrequency(&freq); init = true; }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return now.QuadPart * 1'000'000'000LL / freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<int64>(ts.tv_sec) * 1'000'000'000LL + ts.tv_nsec;
#endif
}

Profiler& Profiler::Get()
{
    static Profiler s_Instance;
    return s_Instance;
}

void Profiler::BeginSample(const std::string& name)
{
    if (!m_Enabled) return;
    m_ActiveSamples[name].startNs = NowNs();
}

void Profiler::EndSample(const std::string& name)
{
    if (!m_Enabled) return;

    auto it = m_ActiveSamples.find(name);
    if (it == m_ActiveSamples.end()) return;

    float64 ms = static_cast<float64>(NowNs() - it->second.startNs) * 1e-6;
    m_ActiveSamples.erase(it);

    auto& entry     = m_Entries[name];
    entry.name      = name;
    entry.totalMs  += ms;
    entry.callCount += 1;

    if (ms < entry.minMs) entry.minMs = ms;
    if (ms > entry.maxMs) entry.maxMs = ms;

    entry.avgMs = entry.totalMs / entry.callCount;
}

void Profiler::BeginFrame()
{
    m_FrameStart = NowNs();
}

void Profiler::EndFrame()
{
    m_FrameMs = static_cast<float64>(NowNs() - m_FrameStart) * 1e-6;
    m_FPS     = m_FrameMs > 0.0 ? static_cast<float32>(1000.0 / m_FrameMs) : 0.0f;
}

void Profiler::ClearEntries()
{
    m_Entries.clear();
}

void Profiler::PrintReport() const
{
    char buf[256];

    std::snprintf(buf, sizeof(buf),
        "=== Profiler Report (Frame %.2fms  FPS %.0f) ===",
        m_FrameMs, static_cast<double>(m_FPS));
    GOOD_LOG_INFO("Profiler", "{}", buf);

    for (auto& [name, entry] : m_Entries)
    {
        std::snprintf(buf, sizeof(buf),
            "  %-20s | avg=%.3fms  min=%.3fms  max=%.3fms  calls=%u",
            name.c_str(),
            entry.avgMs,
            entry.minMs,
            entry.maxMs,
            entry.callCount);
        GOOD_LOG_INFO("Profiler", "{}", buf);
    }
}

} // namespace Good