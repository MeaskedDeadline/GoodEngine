#include "ComponentRegistry.h"

namespace Good
{

    ComponentRegistry& ComponentRegistry::Get()
    {
        static ComponentRegistry s_Instance;
        return s_Instance;
    }

    Ref<Component> ComponentRegistry::Create(const std::string& name) const
    {
        auto it = m_Factories.find(name);
        if (it == m_Factories.end()) return nullptr;
        return it->second();
    }

    bool ComponentRegistry::Has(const std::string& name) const
    {
        return m_Factories.find(name) != m_Factories.end();
    }

} // namespace Good