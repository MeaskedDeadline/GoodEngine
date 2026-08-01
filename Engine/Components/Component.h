#pragma once
#include "../Core/Core.h"

namespace Good
{

    class GameObject;

    class Component
    {
    public:
        virtual ~Component() = default;

        virtual void OnStart()              {}
        virtual void OnUpdate(float32 dt)   {}
        virtual void OnFixedUpdate(float32) {}
        virtual void OnDestroy()            {}

        virtual const char* GetTypeName() const { return "Component"; }

        bool IsEnabled() const { return m_Enabled; }
        void SetEnabled(bool e){ m_Enabled = e; }

        GameObject* GetOwner() const { return m_Owner; }
        void        SetOwner(GameObject* obj) { m_Owner = obj; }

    protected:
        bool        m_Enabled{true};
        GameObject* m_Owner{nullptr};
    };

} // namespace Good