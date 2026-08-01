#pragma once

#include "Core.h"
#include <sstream>
#include <mutex>
#include <fstream>
#include <cstdio>

namespace Good
{

enum class LogLevel : uint8
{
    Trace = 0, Info = 1, Warn = 2, Error = 3, Fatal = 4, Off = 5
};

struct LogRecord
{
    LogLevel    level;
    std::string message;
    std::string category;
    std::string file;
    int32       line{0};
    int64       timestamp{0};
};

class ILogSink
{
public:
    virtual ~ILogSink() = default;
    virtual void Write(const LogRecord& record) = 0;
    virtual void Flush() {}
};

class ConsoleSink final : public ILogSink
{
public:
    void Write(const LogRecord& record) override;
private:
    void SetColor(LogLevel level);
    void ResetColor();
};

class FileSink final : public ILogSink
{
public:
    explicit FileSink(const std::string& path);
    ~FileSink() override;
    void Write(const LogRecord& record) override;
    void Flush() override;
private:
    std::ofstream m_File;
};

class Logger : public NonCopyable
{
public:
    static Logger& Get();

    void     SetLevel(LogLevel level) { m_Level = level; }
    LogLevel GetLevel() const         { return m_Level;  }

    void AddSink(Ref<ILogSink> sink);
    void ClearSinks();
    void Log(LogLevel level, std::string_view category,
             std::string_view message, const char* file, int32 line);

    template<typename... Args>
    std::string Format(std::string_view fmt, Args&&... args)
    {
        std::ostringstream oss;
        FormatImpl(oss, fmt, std::forward<Args>(args)...);
        return oss.str();
    }

private:
    Logger();

    void FormatImpl(std::ostringstream& oss, std::string_view fmt)
    {
        oss << fmt;
    }

    template<typename T, typename... Rest>
    void FormatImpl(std::ostringstream& oss, std::string_view fmt,
                    T&& val, Rest&&... rest)
    {
        auto pos = fmt.find("{}");
        if (pos == std::string_view::npos) { oss << fmt; return; }
        oss << fmt.substr(0, pos) << std::forward<T>(val);
        FormatImpl(oss, fmt.substr(pos + 2), std::forward<Rest>(rest)...);
    }

    std::vector<Ref<ILogSink>> m_Sinks;
    LogLevel                   m_Level{LogLevel::Trace};
    std::mutex                 m_Mutex;
};

} // namespace Good

#define GOOD_LOG_TRACE(cat, ...) ::Good::Logger::Get().Log(::Good::LogLevel::Trace, cat, ::Good::Logger::Get().Format(__VA_ARGS__), __FILE__, __LINE__)
#define GOOD_LOG_INFO(cat, ...)  ::Good::Logger::Get().Log(::Good::LogLevel::Info,  cat, ::Good::Logger::Get().Format(__VA_ARGS__), __FILE__, __LINE__)
#define GOOD_LOG_WARN(cat, ...)  ::Good::Logger::Get().Log(::Good::LogLevel::Warn,  cat, ::Good::Logger::Get().Format(__VA_ARGS__), __FILE__, __LINE__)
#define GOOD_LOG_ERROR(cat, ...) ::Good::Logger::Get().Log(::Good::LogLevel::Error, cat, ::Good::Logger::Get().Format(__VA_ARGS__), __FILE__, __LINE__)
#define GOOD_LOG_FATAL(cat, ...) ::Good::Logger::Get().Log(::Good::LogLevel::Fatal, cat, ::Good::Logger::Get().Format(__VA_ARGS__), __FILE__, __LINE__)