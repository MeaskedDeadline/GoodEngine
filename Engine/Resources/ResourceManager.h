#pragma once
#include "../Core/Core.h"
#include "ResourceCache.h"
#include "ResourceLoader.h"

namespace Good
{

    class ResourceManager : public NonCopyable
    {
    public:
        static ResourceManager& Get();

        void Initialize();
        void Shutdown();

        template<typename T>
        Ref<T> Load(const std::string& path)
        {
            if (ResourceCache::Get().Has(path))
                return ResourceCache::Get().Get<T>(path);

            auto loaded = LoadFromDisk(path);
            if (!loaded) return nullptr;

            auto casted = std::dynamic_pointer_cast<T>(loaded);
            if (casted) ResourceCache::Get().Set<T>(path, casted);
            return casted;
        }

        template<typename T>
        void Register(const std::string& key, Ref<T> resource)
        {
            ResourceCache::Get().Set<T>(key, std::move(resource));
        }

        void AddLoader(Ref<ResourceLoader> loader);
        void Unload(const std::string& path);
        void UnloadAll();

        usize GetLoadedCount() const { return ResourceCache::Get().GetCount(); }

    private:
        ResourceManager() = default;

        Ref<void> LoadFromDisk(const std::string& path);
        std::string GetExtension(const std::string& path) const;

        std::vector<Ref<ResourceLoader>> m_Loaders;
    };

} // namespace Good