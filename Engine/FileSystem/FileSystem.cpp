#include "FileSystem.h"
#include <fstream>
#include <sstream>

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <unistd.h>
    #include <cerrno>
#endif

namespace Good
{

bool FileSystem::Exists(const Path& path)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    return GetFileAttributesA(path.CStr()) != INVALID_FILE_ATTRIBUTES;
#else
    struct stat st;
    return stat(path.CStr(), &st) == 0;
#endif
}

bool FileSystem::IsFile(const Path& path)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    DWORD attr = GetFileAttributesA(path.CStr());
    return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    return stat(path.CStr(), &st) == 0 && S_ISREG(st.st_mode);
#endif
}

bool FileSystem::IsDirectory(const Path& path)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    DWORD attr = GetFileAttributesA(path.CStr());
    return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    return stat(path.CStr(), &st) == 0 && S_ISDIR(st.st_mode);
#endif
}

bool FileSystem::MakeDirectory(const Path& path)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    return ::CreateDirectoryA(path.CStr(), nullptr) ||
           GetLastError() == ERROR_ALREADY_EXISTS;
#else
    return mkdir(path.CStr(), 0755) == 0 || errno == EEXIST;
#endif
}

bool FileSystem::MakeDirectories(const Path& path)
{
    std::string p = path.ToString();
    usize pos = 0;
    while ((pos = p.find('/', pos)) != std::string::npos)
    {
        std::string sub = p.substr(0, pos);
        if (!sub.empty()) MakeDirectory(Path(sub));
        ++pos;
    }
    return MakeDirectory(path);
}

bool FileSystem::Rename(const Path& from, const Path& to)
{
    return std::rename(from.CStr(), to.CStr()) == 0;
}

uint64 FileSystem::FileSize(const Path& path)
{
    std::ifstream file(path.CStr(), std::ios::binary | std::ios::ate);
    if (!file.is_open()) return 0;
    return static_cast<uint64>(file.tellg());
}

std::string FileSystem::ReadTextFile(const Path& path)
{
    std::ifstream file(path.CStr());
    if (!file.is_open()) return "";
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::vector<byte> FileSystem::ReadBinaryFile(const Path& path)
{
    std::ifstream file(path.CStr(), std::ios::binary | std::ios::ate);
    if (!file.is_open()) return {};
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<byte> data(static_cast<usize>(size));
    file.read(reinterpret_cast<char*>(data.data()), size);
    return data;
}

bool FileSystem::WriteTextFile(const Path& path, const std::string& content)
{
    std::ofstream file(path.CStr());
    if (!file.is_open()) return false;
    file << content;
    return true;
}

bool FileSystem::WriteBinaryFile(const Path& path, const std::vector<byte>& data)
{
    std::ofstream file(path.CStr(), std::ios::binary);
    if (!file.is_open()) return false;
    file.write(reinterpret_cast<const char*>(data.data()),
               static_cast<std::streamsize>(data.size()));
    return true;
}

bool FileSystem::AppendTextFile(const Path& path, const std::string& content)
{
    std::ofstream file(path.CStr(), std::ios::app);
    if (!file.is_open()) return false;
    file << content;
    return true;
}

std::string FileSystem::GetWorkingDirectory()
{
    return Path::Current().ToString();
}

bool FileSystem::SetWorkingDirectory(const Path& path)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    return ::SetCurrentDirectoryA(path.CStr()) != 0;
#else
    return ::chdir(path.CStr()) == 0;
#endif
}

std::vector<Path> FileSystem::ListDirectory(const Path& path)
{
    std::vector<Path> result;

#if defined(GOOD_PLATFORM_WINDOWS)
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA((path.ToString() + "/*").c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return result;
    do {
        std::string name = fd.cFileName;
        if (name != "." && name != "..")
            result.push_back(path / name);
    } while (FindNextFileA(h, &fd));
    FindClose(h);
#else
    DIR* dir = opendir(path.CStr());
    if (!dir) return result;
    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr)
    {
        std::string name = ent->d_name;
        if (name != "." && name != "..")
            result.push_back(path / name);
    }
    closedir(dir);
#endif

    return result;
}

std::vector<Path> FileSystem::ListDirectoryRecursive(const Path& path)
{
    std::vector<Path> result;
    auto entries = ListDirectory(path);
    for (auto& entry : entries)
    {
        result.push_back(entry);
        if (IsDirectory(entry))
        {
            auto sub = ListDirectoryRecursive(entry);
            result.insert(result.end(), sub.begin(), sub.end());
        }
    }
    return result;
}

bool FileSystem::Remove(const Path& path)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    if (IsDirectory(path))
        return ::RemoveDirectoryA(path.CStr()) != 0;
    return ::DeleteFileA(path.CStr()) != 0;
#else
    return ::remove(path.CStr()) == 0;
#endif
}

} // namespace Good