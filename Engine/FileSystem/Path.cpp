#include "Path.h"
#include <algorithm>

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace Good
{

char Path::Separator()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    return '\\';
#else
    return '/';
#endif
}

void Path::Normalize(std::string& p)
{
    std::replace(p.begin(), p.end(), '\\', '/');
    // remove double slashes
    usize pos;
    while ((pos = p.find("//")) != std::string::npos)
        p.replace(pos, 2, "/");
}

Path::Path(const std::string& path) : m_Path(path) { Normalize(m_Path); }
Path::Path(const char* path)        : m_Path(path ? path : "") { Normalize(m_Path); }

Path Path::operator/(const std::string& other) const
{
    if (m_Path.empty()) return Path(other);
    if (other.empty())  return *this;
    if (m_Path.back() == '/') return Path(m_Path + other);
    return Path(m_Path + "/" + other);
}

Path& Path::operator/=(const std::string& other)
{
    *this = *this / other;
    return *this;
}

bool Path::operator==(const Path& o) const { return m_Path == o.m_Path; }
bool Path::operator!=(const Path& o) const { return m_Path != o.m_Path; }

std::string Path::GetFilename() const
{
    usize pos = m_Path.rfind('/');
    return (pos == std::string::npos) ? m_Path : m_Path.substr(pos + 1);
}

std::string Path::GetStem() const
{
    std::string fn = GetFilename();
    usize pos = fn.rfind('.');
    return (pos == std::string::npos) ? fn : fn.substr(0, pos);
}

std::string Path::GetExtension() const
{
    std::string fn = GetFilename();
    usize pos = fn.rfind('.');
    return (pos == std::string::npos) ? "" : fn.substr(pos);
}

Path Path::GetParent() const
{
    usize pos = m_Path.rfind('/');
    if (pos == std::string::npos) return Path("");
    return Path(m_Path.substr(0, pos));
}

std::string Path::GetRoot() const
{
    if (m_Path.size() >= 2 && m_Path[1] == ':') return m_Path.substr(0, 3);
    if (!m_Path.empty() && m_Path[0] == '/')    return "/";
    return "";
}

bool Path::IsAbsolute() const
{
    if (m_Path.empty()) return false;
#if defined(GOOD_PLATFORM_WINDOWS)
    return m_Path.size() >= 2 && m_Path[1] == ':';
#else
    return m_Path[0] == '/';
#endif
}

bool Path::IsRelative()   const { return !IsAbsolute(); }
bool Path::HasExtension() const { return !GetExtension().empty(); }

Path Path::WithExtension(const std::string& ext) const
{
    return Path(GetParent().ToString() + "/" + GetStem() + ext);
}

Path Path::WithStem(const std::string& stem) const
{
    return Path(GetParent().ToString() + "/" + stem + GetExtension());
}

Path Path::Current()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    char buf[MAX_PATH]; GetCurrentDirectoryA(MAX_PATH, buf); return Path(buf);
#else
    char buf[4096]; getcwd(buf, sizeof(buf)); return Path(buf);
#endif
}

Path Path::Temp()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    char buf[MAX_PATH]; GetTempPathA(MAX_PATH, buf); return Path(buf);
#else
    return Path("/tmp");
#endif
}

Path Path::Home()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    char buf[MAX_PATH]; GetEnvironmentVariableA("USERPROFILE", buf, MAX_PATH); return Path(buf);
#else
    const char* home = getenv("HOME"); return Path(home ? home : "/");
#endif
}

} // namespace Good