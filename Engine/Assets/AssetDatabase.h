#pragma once
#include "../Core/Core.h"
#include "Asset.h"
#include "../FileSystem/Path.h"
#include "../Utilities/UUID.h"

namespace Good
{

    struct AssetRecord
    {
        UUID        uuid;
        AssetType   type{AssetType::Unknown};
        std::string path;
        std::string name;
        std::string extension;
        uint64      lastModified{0};
    };

    class AssetDatabase : public NonCopyable
    {
    public:
        static AssetDatabase& Get();

        void Scan(const Path& rootPath);
        void Refresh();
        void Clear();

        bool Contains(const UUID& uuid)        const;
        bool ContainsByPath(const std::string& path) const;

        const AssetRecord* Find(const UUID& uuid)         const;
        const AssetRecord* FindByPath(const std::string& path) const;

        std::vector<AssetRecord> GetByType(AssetType type) const;
        const std::vector<AssetRecord>& GetAll() const { return m_Records; }

        usize GetCount() const { return m_Records.size(); }

    private:
        AssetDatabase() = default;

        void ScanDirectory(const Path& path);
        void AddRecord(const Path& file);

        std::vector<AssetRecord>                   m_Records;
        std::unordered_map<std::string, usize>     m_PathIndex;
    };

} // namespace Good