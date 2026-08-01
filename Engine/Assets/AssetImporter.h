#pragma once
#include "../Core/Core.h"
#include "Asset.h"

namespace Good
{

    class AssetImporter
    {
    public:
        virtual ~AssetImporter() = default;
        virtual Ref<Asset> Import(const std::string& path) = 0;
        virtual std::vector<std::string> GetSupportedExtensions() const = 0;
        virtual AssetType GetAssetType() const = 0;
    };

} // namespace Good