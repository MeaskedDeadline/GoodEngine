#include "MemoryTracker.h"
#include "Core/Log.h"
#include <algorithm>

namespace Good
{

    MemoryTracker& MemoryTracker::Get()
    {
        static MemoryTracker s_Instance;
        return s_Instance;
    }

    void MemoryTracker::TrackAlloc(void* ptr, usize size, const char* file, int32 line)
    {
        if (!m_Enabled || !ptr) return;
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Allocations[ptr] = {ptr, size, file, line};
        m_TotalAllocated  += size;
        usize current      = GetCurrentUsage();
        if (current > m_PeakUsage) m_PeakUsage = current;
    }

    void MemoryTracker::TrackFree(void* ptr)
    {
        if (!m_Enabled || !ptr) return;
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto it = m_Allocations.find(ptr);
        if (it != m_Allocations.end())
        {
            m_TotalFreed += it->second.size;
            m_Allocations.erase(it);
        }
    }

    uint32 MemoryTracker::GetLeakCount() const
    {
        return static_cast<uint32>(m_Allocations.size());
    }

    void MemoryTracker::PrintReport() const
    {
        GOOD_LOG_INFO("Memory", "=== Memory Report ===");
        GOOD_LOG_INFO("Memory", "  Allocated : {} bytes", m_TotalAllocated);
        GOOD_LOG_INFO("Memory", "  Freed     : {} bytes", m_TotalFreed);
        GOOD_LOG_INFO("Memory", "  Current   : {} bytes", GetCurrentUsage());
        GOOD_LOG_INFO("Memory", "  Peak      : {} bytes", m_PeakUsage);
        GOOD_LOG_INFO("Memory", "  Leaks     : {}", m_Allocations.size());
    }

    void MemoryTracker::PrintLeaks() const
    {
        if (m_Allocations.empty())
        {
            GOOD_LOG_INFO("Memory", "No memory leaks detected");
            return;
        }
        GOOD_LOG_WARN("Memory", "=== {} MEMORY LEAK(S) ===", m_Allocations.size());
        for (auto& [ptr, info] : m_Allocations)
        {
            GOOD_LOG_WARN("Memory", "  ptr={} size={} | {}:{}",
                reinterpret_cast<uint64>(ptr), info.size, info.file, info.line);
        }
    }

} // namespace Good