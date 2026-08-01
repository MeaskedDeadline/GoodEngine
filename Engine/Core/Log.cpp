#include "Log.h"
#include <chrono>
#include <cstring>

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

namespace Good
{

static int64 CurrentTimeMs()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()).count();
}

static const char* LevelToString(LogLevel l)
{
    switch (l)
    {
        case LogLevel::Trace: return "TRACE";
        case LogLevel::Info:  return "INFO ";
        case LogLevel::Warn:  return "WARN ";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Fatal: return "FATAL";
        default:              return "?????";
    }
}

static std::string BuildLine(const LogRecord& r)
{
    int64 ms   = r.timestamp % 1000;
    int64 secs = (r.timestamp / 1000)    % 60;
    int64 mins = (r.timestamp / 60000)   % 60;
    int64 hrs  = (r.timestamp / 3600000) % 24;

    char t[32];
    std::snprintf(t, sizeof(t), "[%02lld:%02lld:%02lld.%03lld]",
        (long long)hrs, (long long)mins, (long long)secs, (long long)ms);

    const char* fn = r.file.c_str();
    const char* sl = std::strrchr(fn, '/');
    if (!sl) sl    = std::strrchr(fn, '\\');
    if (sl)  fn    = sl + 1;

    std::string out;
    out.reserve(256);
    out += t;
    out += '['; out += LevelToString(r.level); out += ']';
    out += '['; out += r.category;             out += "] ";
    out += r.message;
    out += "  ("; out += fn; out += ':';
    out += std::to_string(r.line); out += ')';
    return out;
}

void ConsoleSink::SetColor(LogLevel level)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD c   = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    switch (level)
    {
        case LogLevel::Trace: c = FOREGROUND_INTENSITY; break;
        case LogLevel::Info:  c = FOREGROUND_GREEN  | FOREGROUND_INTENSITY; break;
        case LogLevel::Warn:  c = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case LogLevel::Error: c = FOREGROUND_RED   | FOREGROUND_INTENSITY; break;
        case LogLevel::Fatal: c = FOREGROUND_RED | FOREGROUND_BLUE  | FOREGROUND_INTENSITY; break;
        default: break;
    }
    SetConsoleTextAttribute(h, c);
#else
    switch (level)
    {
        case LogLevel::Trace: std::printf("\033[90m"); break;
        case LogLevel::Info:  std::printf("\033[32m"); break;
        case LogLevel::Warn:  std::printf("\033[33m"); break;
        case LogLevel::Error: std::printf("\033[31m"); break;
        case LogLevel::Fatal: std::printf("\033[35m"); break;
        default: break;
    }
#endif
}

void ConsoleSink::ResetColor()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    std::printf("\033[0m");
#endif
}

void ConsoleSink::Write(const LogRecord& record)
{
    std::string line = BuildLine(record);
    SetColor(record.level);
    std::puts(line.c_str());
    ResetColor();
}

FileSink::FileSink(const std::string& path)
    : m_File(path, std::ios::out | std::ios::app) {}

FileSink::~FileSink() { Flush(); }

void FileSink::Write(const LogRecord& record)
{
    if (m_File.is_open()) m_File << BuildLine(record) << '\n';
}

void FileSink::Flush()
{
    if (m_File.is_open()) m_File.flush();
}

Logger::Logger() { AddSink(MakeRef<ConsoleSink>()); }

Logger& Logger::Get()
{
    static Logger s_Instance;
    return s_Instance;
}

void Logger::AddSink(Ref<ILogSink> sink)
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Sinks.push_back(std::move(sink));
}

void Logger::ClearSinks()
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Sinks.clear();
}

void Logger::Log(LogLevel level, std::string_view category,
                 std::string_view message, const char* file, int32 line)
{
    if (level < m_Level) return;

    LogRecord rec;
    rec.level     = level;
    rec.message   = message;
    rec.category  = category;
    rec.file      = file ? file : "";
    rec.line      = line;
    rec.timestamp = CurrentTimeMs();

    std::lock_guard<std::mutex> lock(m_Mutex);
    for (auto& sink : m_Sinks) sink->Write(rec);
    if (level >= LogLevel::Error)
        for (auto& sink : m_Sinks) sink->Flush();
}

} // namespace Good