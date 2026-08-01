#include "PluginManager.h"
#include "../Core/Log.h"

namespace Good
{

PluginManager& PluginManager::Get()
{
    static PluginManager s_Instance;
    return s_Instance;
}

void PluginManager::Initialize()
{
    GOOD_LOG_INFO("Plugins", "PluginManager initialized");
}

void PluginManager::Shutdown()
{
    for (auto& [name, plugin] : m_Plugins)
    {
        if (plugin) plugin->OnUnload();
    }
    m_Plugins.clear();
    GOOD_LOG_INFO("Plugins", "PluginManager shutdown");
}

void PluginManager::Update(float32 dt)
{
    for (auto& [name, plugin] : m_Plugins)
    {
        if (plugin && plugin->IsEnabled())
            plugin->OnUpdate(dt);
    }
}

bool PluginManager::LoadPlugin(const std::string& name)
{
    if (m_Plugins.find(name) != m_Plugins.end())
    {
        GOOD_LOG_WARN("Plugins", "Plugin '{}' already loaded", name);
        return false;
    }

    Ref<Plugin> plugin = PluginRegistry::Get().Create(name);
    if (!plugin)
    {
        GOOD_LOG_ERROR("Plugins", "Plugin '{}' not found in registry", name);
        return false;
    }

    if (!plugin->OnLoad())
    {
        GOOD_LOG_ERROR("Plugins", "Plugin '{}' failed to load", name);
        return false;
    }

    m_Plugins[name] = plugin;
    GOOD_LOG_INFO("Plugins", "Plugin '{}' v{} loaded", name, plugin->GetVersion());
    return true;
}

bool PluginManager::UnloadPlugin(const std::string& name)
{
    auto it = m_Plugins.find(name);
    if (it == m_Plugins.end()) return false;

    it->second->OnUnload();
    m_Plugins.erase(it);
    GOOD_LOG_INFO("Plugins", "Plugin '{}' unloaded", name);
    return true;
}

bool PluginManager::ReloadPlugin(const std::string& name)
{
    UnloadPlugin(name);
    return LoadPlugin(name);
}

bool    PluginManager::HasPlugin(const std::string& name) const { return m_Plugins.find(name) != m_Plugins.end(); }
Plugin* PluginManager::GetPlugin(const std::string& name) const
{
    auto it = m_Plugins.find(name);
    return (it != m_Plugins.end()) ? it->second.get() : nullptr;
}

void PluginManager::EnablePlugin(const std::string& name)
{
    auto it = m_Plugins.find(name);
    if (it != m_Plugins.end()) it->second->SetEnabled(true);
}

void PluginManager::DisablePlugin(const std::string& name)
{
    auto it = m_Plugins.find(name);
    if (it != m_Plugins.end()) it->second->SetEnabled(false);
}

} // namespace Good