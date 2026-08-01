#pragma once
#include "../Core/Core.h"
#include "Plugin.h"
#include "PluginRegistry.h"

namespace Good
{

    class PluginManager : public NonCopyable
    {
    public:
        static PluginManager& Get();

        void Initialize();
        void Shutdown();
        void Update(float32 dt);

        bool LoadPlugin(const std::string& name);
        bool UnloadPlugin(const std::string& name);
        bool ReloadPlugin(const std::string& name);

        bool     HasPlugin(const std::string& name) const;
        Plugin*  GetPlugin(const std::string& name) const;

        void EnablePlugin(const std::string& name);
        void DisablePlugin(const std::string& name);

        const std::unordered_map<std::string, Ref<Plugin>>& GetLoadedPlugins() const
        {
            return m_Plugins;
        }

        usize GetPluginCount() const { return m_Plugins.size(); }

    private:
        PluginManager() = default;

        std::unordered_map<std::string, Ref<Plugin>> m_Plugins;
    };

} // namespace Good