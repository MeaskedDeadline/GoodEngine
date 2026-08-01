#include "AssetManager.h"
#include "../Core/Log.h"
#include "../FileSystem/FileSystem.h"
#include "../Utilities/StringUtils.h"

namespace Good
{

AssetManager& AssetManager::Get()
{
    static AssetManager s_Instance;
    return s_Instance;
}

void AssetManager::Initialize(const std::string& projectRoot)
{
    m_ProjectRoot = projectRoot;
    GOOD_LOG_INFO("AssetManager", "Initialized. Root: '{}'", projectRoot);
}

void AssetManager::Shutdown()
{
    UnloadAll();
    GOOD_LOG_INFO("AssetManager", "Shutdown");
}

void AssetManager::Register(Ref<Asset> asset)
{
    if (!asset) return;
    m_PathToAsset[asset->GetPath()] = asset;
    m_UUIDToAsset[asset->GetUUID()] = asset;
}

bool AssetManager::Unload(const std::string& path)
{
    auto it = m_PathToAsset.find(path);
    if (it == m_PathToAsset.end()) return false;

    m_UUIDToAsset.erase(it->second->GetUUID());
    m_PathToAsset.erase(it);
    return true;
}

void AssetManager::UnloadAll()
{
    m_PathToAsset.clear();
    m_UUIDToAsset.clear();
    GOOD_LOG_INFO("AssetManager", "All assets unloaded");
}

usize AssetManager::GetTotalMemory() const
{
    usize total = 0;
    for (auto& [path, asset] : m_PathToAsset)
        if (asset) total += asset->GetMemSize();
    return total;
}

void AssetManager::AddImporter(Ref<AssetImporter> importer)
{
    m_Importers.push_back(std::move(importer));
}

Ref<Asset> AssetManager::ImportAsset(const std::string& path)
{
    std::string fullPath = m_ProjectRoot + "/" + path;
    std::string ext = StringUtils::ToLower(Path(path).GetExtension());

    for (auto& importer : m_Importers)
    {
        for (auto& supported : importer->GetSupportedExtensions())
        {
            if (supported == ext)
            {
                Ref<Asset> asset = importer->Import(fullPath);
                if (asset)
                {
                    asset->SetPath(path);
                    asset->SetName(Path(path).GetStem());
                    asset->SetState(AssetState::Ready);
                    GOOD_LOG_INFO("AssetManager", "Imported '{}' ({})",
                        path, AssetTypeToString(asset->GetType()));
                    return asset;
                }
            }
        }
    }

    GOOD_LOG_WARN("AssetManager", "No importer for '{}'", path);
    return nullptr;
}

} // namespace Good