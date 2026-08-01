#include "ScriptBind.h"
#include "../Core/Log.h"

namespace Good
{

    ScriptBind& ScriptBind::Get()
    {
        static ScriptBind s_Instance;
        return s_Instance;
    }

    void ScriptBind::RegisterFunction(const std::string& name, NativeFunction fn)
    {
        m_Functions[name] = std::move(fn);
        GOOD_LOG_INFO("ScriptBind", "Registered: '{}'", name);
    }

    bool ScriptBind::CallFunction(const std::string& name, const std::vector<std::string>& args)
    {
        auto it = m_Functions.find(name);
        if (it == m_Functions.end())
        {
            GOOD_LOG_WARN("ScriptBind", "Function not found: '{}'", name);
            return false;
        }
        it->second(args);
        return true;
    }

    bool ScriptBind::HasFunction(const std::string& name) const
    {
        return m_Functions.find(name) != m_Functions.end();
    }

} // namespace Good