#pragma once
#include "../Core/Core.h"
#include "Component.h"

namespace Good
{

    using ComponentFactory = std::function<Ref<Component>()>;

    class ComponentRegistry
    {
    public:
        static ComponentRegistry& Get();

        template<typename T>
        void Register(const std::string& name)
        {
            m_Factories[name] = []() -> Ref<Component> { return MakeRef<T>(); };
        }

        Ref<Component> Create(const std::string& name) const;
        bool           Has(const std::string& name) const;

        const std::unordered_map<std::string, ComponentFactory>& GetAll() const { return m_Factories; }

    private:
        ComponentRegistry() = default;
        std::unordered_map<std::string, ComponentFactory> m_Factories;
    };

} // namespace Good