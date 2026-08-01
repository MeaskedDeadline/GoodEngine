#include "Platform.h"

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <psapi.h>
#elif defined(GOOD_PLATFORM_LINUX)
    #include <unistd.h>
    #include <sys/sysinfo.h>
    #include <fstream>
#endif

namespace Good
{

static PlatformInfo s_Info;
static bool         s_Init = false;

static void Build()
{
    if (s_Init) return;
    s_Init = true;

#if defined(GOOD_PLATFORM_WINDOWS)
    s_Info.osName = "Windows";
    SYSTEM_INFO si; GetSystemInfo(&si);
    s_Info.logicalCores = static_cast<uint32>(si.dwNumberOfProcessors);
    MEMORYSTATUSEX mem; mem.dwLength = sizeof(mem); GlobalMemoryStatusEx(&mem);
    s_Info.totalMemoryBytes = static_cast<uint64>(mem.ullTotalPhys);
    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key) == ERROR_SUCCESS)
    {
        char buf[256] = {}; DWORD sz = sizeof(buf);
        RegQueryValueExA(key, "ProcessorNameString", nullptr, nullptr, reinterpret_cast<LPBYTE>(buf), &sz);
        s_Info.cpuName = buf;
        RegCloseKey(key);
    }
#elif defined(GOOD_PLATFORM_LINUX)
    s_Info.osName = "Linux";
    s_Info.logicalCores = static_cast<uint32>(sysconf(_SC_NPROCESSORS_ONLN));
    struct sysinfo si; sysinfo(&si);
    s_Info.totalMemoryBytes = static_cast<uint64>(si.totalram) * si.mem_unit;
#endif
}

const PlatformInfo& Platform::GetInfo() { Build(); return s_Info; }

void Platform::Sleep(uint32 ms)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    ::Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

uint64 Platform::GetMemoryUsage()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return static_cast<uint64>(pmc.WorkingSetSize);
#else
    return 0;
#endif
}

std::string Platform::GetExecutablePath()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    char buf[MAX_PATH]; GetModuleFileNameA(nullptr, buf, MAX_PATH); return buf;
#elif defined(GOOD_PLATFORM_LINUX)
    char buf[4096]; ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf)-1);
    if (len > 0) { buf[len] = '\0'; return buf; }
    return "";
#else
    return "";
#endif
}

} // namespace Good