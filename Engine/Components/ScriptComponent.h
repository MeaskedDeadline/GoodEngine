#pragma once
#include "Component.h"

namespace Good
{

    class ScriptComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "ScriptComponent"; }

        void SetScriptPath(const std::string& path) { m_ScriptPath = path; }
        const std::string& GetScriptPath()    const { return m_ScriptPath; }

    private:
        std::string m_ScriptPath;
    };

} // namespace Good