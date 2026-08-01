#pragma once
#include "../Core/Core.h"

namespace Good
{

    class Path
    {
    public:
        Path() = default;
        explicit Path(const std::string& path);
        explicit Path(const char* path);

        Path  operator/(const std::string& other) const;
        Path& operator/=(const std::string& other);
        bool  operator==(const Path& other) const;
        bool  operator!=(const Path& other) const;

        std::string ToString()    const { return m_Path; }
        const char* CStr()        const { return m_Path.c_str(); }

        std::string GetFilename() const;
        std::string GetStem()     const;
        std::string GetExtension()const;
        Path        GetParent()   const;
        std::string GetRoot()     const;

        bool IsAbsolute()   const;
        bool IsRelative()   const;
        bool HasExtension() const;
        bool IsEmpty()      const { return m_Path.empty(); }

        Path WithExtension(const std::string& ext) const;
        Path WithStem(const std::string& stem)     const;

        static Path Current();
        static Path Temp();
        static Path Home();
        static char Separator();
        static void Normalize(std::string& path);

    private:
        std::string m_Path;
    };

} // namespace Good