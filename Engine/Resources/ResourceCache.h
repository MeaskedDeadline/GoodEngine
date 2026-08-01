#pragma once
#include "../Core/Core.h"
#include <typeindex>

namespace Good
{

    class ResourceCache : public NonCopyable
    {
    public:
        static ResourceCache& Get();

        template<typename T>
        void Set(const std::string& key, Ref<T> resource)
        {
            m_Cache[key] = std::static_pointer_cast<void>(std::move(resource));
            m_TypeMap[key] = std::type_index(typeid(T));
        }

        template<typename T>
        Ref<T> Get(const std::string& key) const
        {
            auto it = m_Cache.find(key);
            if (it == m_Cache.end()) return nullptr;
            auto tit = m_TypeMap.find(key);
            if (tit == m_TypeMap.end() || tit->second != std::type_index(typeid(T))) return nullptr;
            return std::static_pointer_cast<T>(it->second);
        }

        bool Has(const std::string& key) const;
        bool Remove(const std::string& key);
        void Clear();

        usize GetCount()   const { return m_Cache.size(); }

    private:
        ResourceCache() = default;

        std::unordered_map<std::string, std::shared_ptr<void>> m_Cache;
        std::unordered_map<std::string, std::type_index>       m_TypeMap;
    };

} // namespace Good