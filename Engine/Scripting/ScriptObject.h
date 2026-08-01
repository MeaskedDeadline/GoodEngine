#pragma once
#include "../Core/Core.h"

namespace Good
{

    class ScriptObject
    {
    public:
        virtual ~ScriptObject() = default;

        virtual void OnCreate()            {}
        virtual void OnStart()             {}
        virtual void OnUpdate(float32 dt)  {}
        virtual void OnFixedUpdate(float32){}
        virtual void OnDestroy()           {}
        virtual void OnCollision()         {}
        virtual void OnTriggerEnter()      {}
        virtual void OnTriggerExit()       {}

        virtual const char* GetScriptName() const { return "ScriptObject"; }

        bool IsEnabled() const { return m_Enabled; }
        void SetEnabled(bool e){ m_Enabled = e; }

    protected:
        bool m_Enabled{true};
    };

} // namespace Good