#pragma once
#include "../Core/Core.h"

namespace Good
{

    struct PlatformInfo
    {
        std::string osName;
        std::string cpuName;
        uint32      logicalCores{0};
        uint64      totalMemoryBytes{0};
    };

    class Platform
    {
    public:
        static const PlatformInfo& GetInfo();
        static void                Sleep(uint32 milliseconds);
        static uint64              GetMemoryUsage();
        static std::string         GetExecutablePath();
    };

} // namespace Good