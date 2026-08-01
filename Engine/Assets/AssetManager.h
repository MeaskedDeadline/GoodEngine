#pragma once
#include "../Core/Core.h"
#include "Asset.h"
#include "AssetImporter.h"
#include "../Utilities/UUID.h"
#include <typeindex>

namespace Good
{

    class AssetManager : public NonCopyable
    {
    public:
        static AssetManager& Get();

        void Initialize(const std::string& projectRoot = "Assets");
        void Shutdown();

        template<typename T>
        Ref<T> Load(const std::string& path)
        {
            auto it = m_PathToAsset.find(path);
            if (it != m_PathToAsset.end())
                return std::dynamic_pointer_cast<T>(it->second);

            auto asset = ImportAsset(path);
            if (!asset) return nullptr;

            m_PathToAsset[path] = asset;
            m_UUIDToAsset[asset->GetUUID()] = asset;
            return std::dynamic_pointer_cast<T>(asset);
        }

        template<typename T>
        Ref<T> GetByUUID(const UUID& uuid)
        {
            auto it = m_UUIDToAsset.find(uuid);
            return (it != m_UUIDToAsset.end())
                ? std::dynamic_pointer_cast<T>(it->second) : nullptr;
        }

        void Register(Ref<Asset> asset);
        bool Unload(const std::string& path);
        void UnloadAll();

        usize GetLoadedCount() const { return m_PathToAsset.size(); }
        usize GetTotalMemory() const;

        void AddImporter(Ref<AssetImporter> importer);

    private:
        AssetManager() = default;

        Ref<Asset> ImportAsset(const std::string& path);

        struct UUIDHash
        {
            usize operator()(const UUID& u) const
            {
                return UUID::Hash{}(u);
            }
        };

        std::unordered_map<std::string, Ref<Asset>> m_PathToAsset;
        std::unordered_map<UUID, Ref<Asset>, UUIDHash> m_UUIDToAsset;
        std::vector<Ref<AssetImporter>>              m_Importers;
        std::string                                  m_ProjectRoot;
    };

} // namespace Good