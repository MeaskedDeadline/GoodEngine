#pragma once
#include "../Core/Core.h"
#include "ScriptRuntime.h"
#include "ScriptBind.h"

namespace Good
{

    class ScriptEngine : public NonCopyable
    {
    public:
        static ScriptEngine& Get();

        bool Initialize();
        void Shutdown();
        void Update(float32 dt);

        ScriptRuntime& GetRuntime() { return ScriptRuntime::Get(); }
        ScriptBind&    GetBind()    { return ScriptBind::Get();    }

        void RegisterBuiltins();

    private:
        ScriptEngine() = default;
        bool m_Initialized{false};
    };

} // namespace Good