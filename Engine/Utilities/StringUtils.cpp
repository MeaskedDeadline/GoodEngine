#include "StringUtils.h"
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>

namespace Good
{

std::string StringUtils::ToLower(const std::string& s)
{
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return r;
}

std::string StringUtils::ToUpper(const std::string& s)
{
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](unsigned char c){ return static_cast<char>(std::toupper(c)); });
    return r;
}

std::string StringUtils::TrimLeft(const std::string& s)
{
    usize i = 0;
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    return s.substr(i);
}

std::string StringUtils::TrimRight(const std::string& s)
{
    isize i = static_cast<isize>(s.size()) - 1;
    while (i >= 0 && std::isspace(static_cast<unsigned char>(s[i]))) --i;
    return s.substr(0, static_cast<usize>(i + 1));
}

std::string StringUtils::Trim(const std::string& s)
{
    return TrimLeft(TrimRight(s));
}

bool StringUtils::StartsWith(const std::string& s, const std::string& prefix)
{
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

bool StringUtils::EndsWith(const std::string& s, const std::string& suffix)
{
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool StringUtils::Contains(const std::string& s, const std::string& sub)
{
    return s.find(sub) != std::string::npos;
}

std::vector<std::string> StringUtils::Split(const std::string& s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delim))
        result.push_back(token);
    return result;
}

std::vector<std::string> StringUtils::Split(const std::string& s, const std::string& delim)
{
    std::vector<std::string> result;
    usize start = 0, pos = 0;
    while ((pos = s.find(delim, start)) != std::string::npos)
    {
        result.push_back(s.substr(start, pos - start));
        start = pos + delim.size();
    }
    result.push_back(s.substr(start));
    return result;
}

std::string StringUtils::Join(const std::vector<std::string>& parts, const std::string& sep)
{
    std::string result;
    for (usize i = 0; i < parts.size(); ++i)
    {
        result += parts[i];
        if (i + 1 < parts.size()) result += sep;
    }
    return result;
}

std::string StringUtils::Replace(const std::string& s, const std::string& from, const std::string& to)
{
    std::string result = s;
    usize pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos)
    {
        result.replace(pos, from.size(), to);
        pos += to.size();
    }
    return result;
}

int32   StringUtils::ToInt(const std::string& s, int32 def)
{
    try { return std::stoi(s); }
    catch (...) { return def; }
}

float32 StringUtils::ToFloat(const std::string& s, float32 def)
{
    try { return std::stof(s); }
    catch (...) { return def; }
}

bool StringUtils::ToBool(const std::string& s, bool def)
{
    std::string l = ToLower(Trim(s));
    if (l == "true"  || l == "1" || l == "yes") return true;
    if (l == "false" || l == "0" || l == "no")  return false;
    return def;
}

std::string StringUtils::FromInt(int32 v)             { return std::to_string(v); }
std::string StringUtils::FromBool(bool v)             { return v ? "true" : "false"; }

std::string StringUtils::FromFloat(float32 v, int32 precision)
{
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.*f", precision, static_cast<double>(v));
    return buf;
}

bool StringUtils::IsNumber(const std::string& s)
{
    if (s.empty()) return false;
    usize start = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    bool hasDot = false;
    for (usize i = start; i < s.size(); ++i)
    {
        if (s[i] == '.' && !hasDot) { hasDot = true; continue; }
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return start < s.size();
}

bool StringUtils::IsEmpty(const std::string& s) { return Trim(s).empty(); }

usize StringUtils::Count(const std::string& s, char c)
{
    return static_cast<usize>(std::count(s.begin(), s.end(), c));
}

std::string StringUtils::Repeat(const std::string& s, uint32 n)
{
    std::string result;
    result.reserve(s.size() * n);
    for (uint32 i = 0; i < n; ++i) result += s;
    return result;
}

std::string StringUtils::PadLeft(const std::string& s, usize width, char fill)
{
    if (s.size() >= width) return s;
    return std::string(width - s.size(), fill) + s;
}

std::string StringUtils::PadRight(const std::string& s, usize width, char fill)
{
    if (s.size() >= width) return s;
    return s + std::string(width - s.size(), fill);
}

std::string StringUtils::Format(const std::string& fmt, ...)
{
    char buf[4096];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt.c_str(), args);
    va_end(args);
    return buf;
}

} // namespace Good