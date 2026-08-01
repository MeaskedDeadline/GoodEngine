#include "VirtualFileSystem.h"
#include "FileSystem.h"
#include "../Core/Log.h"
#include "../Utilities/StringUtils.h"

namespace Good
{

    VirtualFileSystem& VirtualFileSystem::Get()
    {
        static VirtualFileSystem s_Instance;
        return s_Instance;
    }

    void VirtualFileSystem::Mount(const std::string& vPath, const Path& realPath)
    {
        m_Mounts[vPath] = realPath;
        GOOD_LOG_INFO("VFS", "Mounted '{}' -> '{}'", vPath, realPath.ToString());
    }

    void VirtualFileSystem::Unmount(const std::string& vPath)
    {
        m_Mounts.erase(vPath);
        GOOD_LOG_INFO("VFS", "Unmounted '{}'", vPath);
    }

    Path VirtualFileSystem::Resolve(const std::string& vPath) const
    {
        for (auto& [mount, real] : m_Mounts)
        {
            if (StringUtils::StartsWith(vPath, mount))
            {
                std::string relative = vPath.substr(mount.size());
                if (!relative.empty() && (relative[0] == '/' || relative[0] == '\\'))
                    relative = relative.substr(1);
                return real / relative;
            }
        }
        return Path(vPath);
    }

    bool VirtualFileSystem::Exists(const std::string& vPath) const
    {
        return FileSystem::Exists(Resolve(vPath));
    }

    std::vector<byte> VirtualFileSystem::ReadBinary(const std::string& vPath) const
    {
        return FileSystem::ReadBinaryFile(Resolve(vPath));
    }

    std::string VirtualFileSystem::ReadText(const std::string& vPath) const
    {
        return FileSystem::ReadTextFile(Resolve(vPath));
    }

} // namespace Good