#pragma once
#include "Core/Core.h"
#include "Path.h"

namespace Good
{

    class FileSystem
    {
    public:
        static bool Exists(const Path& path);
        static bool IsFile(const Path& path);
        static bool IsDirectory(const Path& path);

        static bool MakeDirectory(const Path& path);
        static bool MakeDirectories(const Path& path);

        static bool Remove(const Path& path);
        static bool Rename(const Path& from, const Path& to);

        static uint64 FileSize(const Path& path);

        static std::vector<Path> ListDirectory(const Path& path);
        static std::vector<Path> ListDirectoryRecursive(const Path& path);

        static std::string        ReadTextFile(const Path& path);
        static std::vector<byte>  ReadBinaryFile(const Path& path);
        static bool               WriteTextFile(const Path& path, const std::string& content);
        static bool               WriteBinaryFile(const Path& path, const std::vector<byte>& data);
        static bool               AppendTextFile(const Path& path, const std::string& content);

        static std::string GetWorkingDirectory();
        static bool        SetWorkingDirectory(const Path& path);
    };

} // namespace Good