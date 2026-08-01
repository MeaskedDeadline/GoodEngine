#include "PluginRegistry.h"

namespace Good
{

    PluginRegistry& PluginRegistry::Get()
    {
        static PluginRegistry s_Instance;
        return s_Instance;
    }

    Ref<Plugin> PluginRegistry::Create(const std::string& name) const
    {
        auto it = m_Factories.find(name);
        return (it != m_Factories.end()) ? it->second() : nullptr;
    }

    bool PluginRegistry::HasPlugin(const std::string& name) const
    {
        return m_Factories.find(name) != m_Factories.end();
    }

} // namespace Good