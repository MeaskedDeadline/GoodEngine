#pragma once
#include "Panel.h"
#include "Debug/Profiler.h"

namespace Good
{

    class ProfilerPanel final : public Panel
    {
    public:
        ProfilerPanel();

        void OnRender() override;
        void OnUpdate(float32 dt) override;
        const char* GetTitle() const override { return "Profiler"; }

    private:
        float32 m_UpdateTimer{0.0f};
        float32 m_UpdateInterval{0.5f};
        std::vector<std::pair<std::string, ProfilerEntry>> m_CachedEntries;
    };

} // namespace Good