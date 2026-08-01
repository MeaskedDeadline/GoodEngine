#pragma once
#include "Core.h"

namespace Good
{

    class Time
    {
    public:
        static void    Initialize();
        static void    Tick();

        static float32 DeltaTime()      { return s_DeltaTime;      }
        static float32 UnscaledDelta()  { return s_UnscaledDelta;  }
        static float64 Elapsed()        { return s_Elapsed;        }
        static uint64  FrameCount()     { return s_FrameCount;     }
        static float32 FPS()            { return s_FPS;            }
        static float32 TimeScale()      { return s_TimeScale;      }
        static void    SetTimeScale(float32 s) { s_TimeScale = s;  }
        static float32 FixedDeltaTime() { return s_FixedDeltaTime; }
        static void    SetFixedDeltaTime(float32 d) { s_FixedDeltaTime = d; }

    private:
        static float32 s_DeltaTime;
        static float32 s_UnscaledDelta;
        static float64 s_Elapsed;
        static float64 s_LastTime;
        static uint64  s_FrameCount;
        static float32 s_FPS;
        static float32 s_TimeScale;
        static float32 s_FixedDeltaTime;
        static float64 s_FPSTimer;
        static uint32  s_FPSFrameCount;
    };

    class Stopwatch
    {
    public:
        Stopwatch();
        void    Start();
        void    Stop();
        void    Reset();
        float64 ElapsedSeconds()      const;
        float64 ElapsedMilliseconds() const;
        bool    IsRunning() const { return m_Running; }
    private:
        static int64 NowNs();
        int64 m_StartNs{0};
        int64 m_AccumNs{0};
        bool  m_Running{false};
    };

} // namespace Good