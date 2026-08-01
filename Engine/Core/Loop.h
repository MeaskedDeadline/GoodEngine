#pragma once
#include "Core.h"

namespace Good
{
    class App;

    class Loop : public NonCopyable
    {
    public:
        Loop()  = default;
        ~Loop() = default;
        void Run(Scope<App> app);
    private:
        static constexpr uint32 MaxFixedSteps = 8;
        float32 m_Accumulator{0.0f};
    };
} // namespace Good