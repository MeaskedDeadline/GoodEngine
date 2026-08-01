#pragma once
#include "../Core/Core.h"
#include "Plugin.h"
#include <functional>

namespace Good
{

    using PluginFactory = std::function<Ref<Plugin>()>;

    class PluginRegistry : public NonCopyable
    {
    public:
        static PluginRegistry& Get();

        template<typename T>
        void Register(const std::string& name)
        {
            m_Factories[name] = []() -> Ref<Plugin> { return MakeRef<T>(); };
        }

        Ref<Plugin> Create(const std::string& name) const;
        bool        HasPlugin(const std::string& name) const;

        const std::unordered_map<std::string, PluginFactory>& GetAll() const { return m_Factories; }

    private:
        PluginRegistry() = default;
        std::unordered_map<std::string, PluginFactory> m_Factories;
    };

} // namespace Good