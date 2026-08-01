#pragma once
#include "../Core/Core.h"

namespace Good
{

    class StringUtils
    {
    public:
        static std::string  ToLower(const std::string& s);
        static std::string  ToUpper(const std::string& s);
        static std::string  Trim(const std::string& s);
        static std::string  TrimLeft(const std::string& s);
        static std::string  TrimRight(const std::string& s);
        static bool         StartsWith(const std::string& s, const std::string& prefix);
        static bool         EndsWith(const std::string& s, const std::string& suffix);
        static bool         Contains(const std::string& s, const std::string& sub);
        static std::vector<std::string> Split(const std::string& s, char delimiter);
        static std::vector<std::string> Split(const std::string& s, const std::string& delimiter);
        static std::string  Join(const std::vector<std::string>& parts, const std::string& sep);
        static std::string  Replace(const std::string& s, const std::string& from, const std::string& to);
        static std::string  Format(const std::string& fmt, ...);
        static int32        ToInt(const std::string& s, int32 def = 0);
        static float32      ToFloat(const std::string& s, float32 def = 0.0f);
        static bool         ToBool(const std::string& s, bool def = false);
        static std::string  FromInt(int32 v);
        static std::string  FromFloat(float32 v, int32 precision = 3);
        static std::string  FromBool(bool v);
        static bool         IsNumber(const std::string& s);
        static bool         IsEmpty(const std::string& s);
        static usize        Count(const std::string& s, char c);
        static std::string  Repeat(const std::string& s, uint32 n);
        static std::string  PadLeft(const std::string& s, usize width, char fill = ' ');
        static std::string  PadRight(const std::string& s, usize width, char fill = ' ');
    };

} // namespace Good