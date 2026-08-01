#pragma once
#include "../Core/Core.h"
#include "Path.h"

namespace Good
{

    class VirtualFileSystem : public NonCopyable
    {
    public:
        static VirtualFileSystem& Get();

        void Mount(const std::string& virtualPath, const Path& realPath);
        void Unmount(const std::string& virtualPath);

        Path Resolve(const std::string& virtualPath) const;
        bool Exists(const std::string& virtualPath)  const;

        std::vector<byte> ReadBinary(const std::string& virtualPath) const;
        std::string       ReadText(const std::string& virtualPath)   const;

    private:
        VirtualFileSystem() = default;

        std::unordered_map<std::string, Path> m_Mounts;
    };

} // namespace Good