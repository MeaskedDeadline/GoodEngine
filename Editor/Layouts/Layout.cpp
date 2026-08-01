#include "Layouts/Layout.h"

namespace Good
{

    const LayoutRegion* Layout::GetRegion(const std::string& name) const
    {
        for (auto& r : m_Regions)
            if (r.name == name) return &r;
        return nullptr;
    }

    LayoutRegion* Layout::GetRegion(const std::string& name)
    {
        for (auto& r : m_Regions)
            if (r.name == name) return &r;
        return nullptr;
    }

    void Layout::SetRegion(const LayoutRegion& region)
    {
        for (auto& r : m_Regions)
        {
            if (r.name == region.name) { r = region; return; }
        }
        m_Regions.push_back(region);
    }

    void Layout::AddRegion(const LayoutRegion& region)
    {
        m_Regions.push_back(region);
    }

} // namespace Good