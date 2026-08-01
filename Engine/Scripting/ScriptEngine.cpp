#include "ScriptEngine.h"
#include "../Core/Log.h"
#include "../Core/Time.h"

namespace Good
{

    ScriptEngine& ScriptEngine::Get()
    {
        static ScriptEngine s_Instance;
        return s_Instance;
    }

    bool ScriptEngine::Initialize()
    {
        if (m_Initialized) return true;

        ScriptRuntime::Get().Initialize();
        RegisterBuiltins();

        m_Initialized = true;
        GOOD_LOG_INFO("Scripting", "ScriptEngine initialized");
        return true;
    }

    void ScriptEngine::Shutdown()
    {
        ScriptRuntime::Get().Shutdown();
        m_Initialized = false;
        GOOD_LOG_INFO("Scripting", "ScriptEngine shutdown");
    }

    void ScriptEngine::Update(float32 dt)
    {
        ScriptRuntime::Get().Update(dt);
    }

    void ScriptEngine::RegisterBuiltins()
    {
        auto& bind = ScriptBind::Get();

        bind.RegisterFunction("print", [](const std::vector<std::string>& args)
        {
            for (auto& a : args) GOOD_LOG_INFO("Script", "{}", a);
        });

        bind.RegisterFunction("log.warn", [](const std::vector<std::string>& args)
        {
            for (auto& a : args) GOOD_LOG_WARN("Script", "{}", a);
        });

        bind.RegisterFunction("time.dt", [](const std::vector<std::string>&)
        {
            GOOD_LOG_INFO("Script", "DeltaTime: {}", Time::DeltaTime());
        });

        GOOD_LOG_INFO("Scripting", "Builtins registered");
    }

} // namespace Good