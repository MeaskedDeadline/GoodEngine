#pragma once
#include "../Core/Core.h"
#include "ScriptObject.h"

namespace Good
{

    class ScriptRuntime : public NonCopyable
    {
    public:
        static ScriptRuntime& Get();

        bool Initialize();
        void Shutdown();
        void Update(float32 dt);
        void FixedUpdate(float32 dt);

        Ref<ScriptObject> CreateScript(const std::string& className);
        void              DestroyScript(Ref<ScriptObject> script);

        bool ExecuteFile(const std::string& path);
        bool ExecuteString(const std::string& code);

        bool IsInitialized() const { return m_Initialized; }

    private:
        ScriptRuntime() = default;

        std::vector<Ref<ScriptObject>> m_Scripts;
        bool m_Initialized{false};
    };

} // namespace Good