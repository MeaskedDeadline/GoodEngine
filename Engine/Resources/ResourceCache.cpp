#include "ResourceCache.h"

namespace Good
{

    ResourceCache& ResourceCache::Get()
    {
        static ResourceCache s_Instance;
        return s_Instance;
    }

    bool ResourceCache::Has(const std::string& key) const
    {
        return m_Cache.find(key) != m_Cache.end();
    }

    bool ResourceCache::Remove(const std::string& key)
    {
        auto it = m_Cache.find(key);
        if (it == m_Cache.end()) return false;
        m_TypeMap.erase(key);
        m_Cache.erase(it);
        return true;
    }

    void ResourceCache::Clear()
    {
        m_Cache.clear();
        m_TypeMap.clear();
    }

} // namespace Good