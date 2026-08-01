#pragma once
#include "../Core/Core.h"

namespace Good
{

    struct AllocationInfo
    {
        void*       ptr{nullptr};
        usize       size{0};
        const char* file{"?"};
        int32       line{0};
    };

    class MemoryTracker : public NonCopyable
    {
    public:
        static MemoryTracker& Get();

        void Enable()  { m_Enabled = true;  }
        void Disable() { m_Enabled = false; }
        bool IsEnabled() const { return m_Enabled; }

        void TrackAlloc(void* ptr, usize size, const char* file, int32 line);
        void TrackFree(void* ptr);

        usize GetTotalAllocated()  const { return m_TotalAllocated; }
        usize GetTotalFreed()      const { return m_TotalFreed;     }
        usize GetCurrentUsage()    const { return m_TotalAllocated - m_TotalFreed; }
        usize GetPeakUsage()       const { return m_PeakUsage;      }
        uint32 GetLeakCount()      const;

        void PrintReport() const;
        void PrintLeaks()  const;

    private:
        MemoryTracker() = default;

        bool m_Enabled{false};
        usize m_TotalAllocated{0};
        usize m_TotalFreed{0};
        usize m_PeakUsage{0};

        std::unordered_map<void*, AllocationInfo> m_Allocations;
        std::mutex m_Mutex;
    };

} // namespace Good