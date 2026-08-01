#include "ResourceManager.h"
#include "../Core/Log.h"
#include "../Utilities/StringUtils.h"
#include "../FileSystem/Path.h"

namespace Good
{

    ResourceManager& ResourceManager::Get()
    {
        static ResourceManager s_Instance;
        return s_Instance;
    }

    void ResourceManager::Initialize()
    {
        GOOD_LOG_INFO("ResourceManager", "Initialized");
    }

    void ResourceManager::Shutdown()
    {
        UnloadAll();
        GOOD_LOG_INFO("ResourceManager", "Shutdown");
    }

    void ResourceManager::AddLoader(Ref<ResourceLoader> loader)
    {
        m_Loaders.push_back(std::move(loader));
    }

    void ResourceManager::Unload(const std::string& path)
    {
        ResourceCache::Get().Remove(path);
    }

    void ResourceManager::UnloadAll()
    {
        ResourceCache::Get().Clear();
    }

    Ref<void> ResourceManager::LoadFromDisk(const std::string& path)
    {
        std::string ext = StringUtils::ToLower(Path(path).GetExtension());

        for (auto& loader : m_Loaders)
        {
            if (loader->CanLoad(ext))
            {
                auto res = loader->Load(path);
                if (res)
                {
                    GOOD_LOG_INFO("ResourceManager", "Loaded '{}'", path);
                    return res;
                }
            }
        }

        GOOD_LOG_WARN("ResourceManager", "Cannot load '{}'", path);
        return nullptr;
    }

} // namespace Good