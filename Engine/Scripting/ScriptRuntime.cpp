#include "ScriptRuntime.h"
#include "../Core/Log.h"

namespace Good
{

    ScriptRuntime& ScriptRuntime::Get()
    {
        static ScriptRuntime s_Instance;
        return s_Instance;
    }

    bool ScriptRuntime::Initialize()
    {
        if (m_Initialized) return true;
        m_Initialized = true;
        GOOD_LOG_INFO("Scripting", "ScriptRuntime initialized (stub)");
        return true;
    }

    void ScriptRuntime::Shutdown()
    {
        m_Scripts.clear();
        m_Initialized = false;
        GOOD_LOG_INFO("Scripting", "ScriptRuntime shutdown");
    }

    void ScriptRuntime::Update(float32 dt)
    {
        for (auto& s : m_Scripts)
            if (s && s->IsEnabled()) s->OnUpdate(dt);
    }

    void ScriptRuntime::FixedUpdate(float32 dt)
    {
        for (auto& s : m_Scripts)
            if (s && s->IsEnabled()) s->OnFixedUpdate(dt);
    }

    Ref<ScriptObject> ScriptRuntime::CreateScript(const std::string& className)
    {
        GOOD_LOG_INFO("Scripting", "Create script: '{}'", className);
        auto script = MakeRef<ScriptObject>();
        m_Scripts.push_back(script);
        script->OnCreate();
        return script;
    }

    void ScriptRuntime::DestroyScript(Ref<ScriptObject> script)
    {
        if (script) script->OnDestroy();
        m_Scripts.erase(
            std::remove(m_Scripts.begin(), m_Scripts.end(), script),
            m_Scripts.end());
    }

    bool ScriptRuntime::ExecuteFile(const std::string& path)
    {
        GOOD_LOG_INFO("Scripting", "ExecuteFile: '{}' (stub)", path);
        return false;
    }

    bool ScriptRuntime::ExecuteString(const std::string& code)
    {
        GOOD_LOG_INFO("Scripting", "ExecuteString (len={}) (stub)", code.size());
        return false;
    }

} // namespace Good