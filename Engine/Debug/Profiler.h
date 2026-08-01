#pragma once
#include "../Core/Core.h"

namespace Good
{

    struct ProfilerEntry
    {
        std::string name;
        float64     totalMs{0.0};
        float64     avgMs{0.0};
        float64     minMs{999999.0};
        float64     maxMs{0.0};
        uint32      callCount{0};
    };

    class Profiler : public NonCopyable
    {
    public:
        static Profiler& Get();

        void Enable()  { m_Enabled = true;  }
        void Disable() { m_Enabled = false; }
        bool IsEnabled() const { return m_Enabled; }

        void BeginSample(const std::string& name);
        void EndSample(const std::string& name);

        void BeginFrame();
        void EndFrame();

        const std::unordered_map<std::string, ProfilerEntry>& GetEntries() const { return m_Entries; }
        void ClearEntries();
        void PrintReport() const;

        float64 GetFrameMs() const { return m_FrameMs; }
        float32 GetFPS()     const { return m_FPS;     }

    private:
        Profiler() = default;

        struct Sample
        {
            int64 startNs{0};
        };

        static int64 NowNs();

        std::unordered_map<std::string, ProfilerEntry> m_Entries;
        std::unordered_map<std::string, Sample>        m_ActiveSamples;

        int64   m_FrameStart{0};
        float64 m_FrameMs{0.0};
        float32 m_FPS{0.0f};
        bool    m_Enabled{false};
    };

} // namespace Good