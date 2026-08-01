#include "AssetDatabase.h"
#include "../FileSystem/FileSystem.h"
#include "../Core/Log.h"
#include "../Utilities/StringUtils.h"

namespace Good
{

AssetDatabase& AssetDatabase::Get()
{
    static AssetDatabase s_Instance;
    return s_Instance;
}

void AssetDatabase::Scan(const Path& root)
{
    Clear();
    ScanDirectory(root);
    GOOD_LOG_INFO("AssetDatabase", "Scanned {} assets in '{}'",
        m_Records.size(), root.ToString());
}

void AssetDatabase::Refresh()
{
    GOOD_LOG_INFO("AssetDatabase", "Database refreshed ({} assets)", m_Records.size());
}

void AssetDatabase::Clear()
{
    m_Records.clear();
    m_PathIndex.clear();
}

void AssetDatabase::ScanDirectory(const Path& path)
{
    auto entries = FileSystem::ListDirectory(path);
    for (auto& entry : entries)
    {
        if (FileSystem::IsDirectory(entry))
            ScanDirectory(entry);
        else
            AddRecord(entry);
    }
}

void AssetDatabase::AddRecord(const Path& file)
{
    std::string ext  = StringUtils::ToLower(file.GetExtension());
    AssetType   type = AssetTypeFromExtension(ext);

    if (type == AssetType::Unknown) return;

    AssetRecord rec;
    rec.uuid      = UUID::Generate();
    rec.type      = type;
    rec.path      = file.ToString();
    rec.name      = file.GetStem();
    rec.extension = ext;

    m_PathIndex[rec.path] = m_Records.size();
    m_Records.push_back(rec);
}

bool AssetDatabase::Contains(const UUID& uuid) const
{
    for (auto& r : m_Records) if (r.uuid == uuid) return true;
    return false;
}

bool AssetDatabase::ContainsByPath(const std::string& path) const
{
    return m_PathIndex.find(path) != m_PathIndex.end();
}

const AssetRecord* AssetDatabase::Find(const UUID& uuid) const
{
    for (auto& r : m_Records) if (r.uuid == uuid) return &r;
    return nullptr;
}

const AssetRecord* AssetDatabase::FindByPath(const std::string& path) const
{
    auto it = m_PathIndex.find(path);
    return (it != m_PathIndex.end()) ? &m_Records[it->second] : nullptr;
}

std::vector<AssetRecord> AssetDatabase::GetByType(AssetType type) const
{
    std::vector<AssetRecord> result;
    for (auto& r : m_Records) if (r.type == type) result.push_back(r);
    return result;
}

} // namespace Good