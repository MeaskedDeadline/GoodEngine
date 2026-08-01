#pragma once
#include "Profiler.h"

namespace Good
{

    class ProfilerScope
    {
    public:
        explicit ProfilerScope(const char* name)
            : m_Name(name)
        {
            Profiler::Get().BeginSample(m_Name);
        }

        ~ProfilerScope()
        {
            Profiler::Get().EndSample(m_Name);
        }

    private:
        std::string m_Name;
    };

} // namespace Good

#define GOOD_PROFILE_SCOPE(name) ::Good::ProfilerScope _profileScope##__LINE__(name)
#define GOOD_PROFILE_FUNCTION()  ::Good::ProfilerScope _profileScopeFunc(__FUNCTION__)