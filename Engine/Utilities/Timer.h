#pragma once
#include "../Core/Core.h"

namespace Good
{

    class Timer
    {
    public:
        Timer();

        void    Start();
        void    Stop();
        void    Reset();
        void    Restart();

        float64 ElapsedSeconds()      const;
        float64 ElapsedMilliseconds() const;
        float64 ElapsedMicroseconds() const;

        bool IsRunning() const { return m_Running; }

    private:
        static int64 NowNs();

        int64 m_StartNs{0};
        int64 m_AccumNs{0};
        bool  m_Running{false};
    };

} // namespace Good