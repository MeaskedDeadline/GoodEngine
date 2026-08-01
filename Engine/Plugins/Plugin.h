#pragma once
#include "../Core/Core.h"

namespace Good
{

    class Plugin
    {
    public:
        virtual ~Plugin() = default;

        virtual bool OnLoad()   { return true; }
        virtual void OnUnload() {}
        virtual void OnUpdate(float32 dt) {}

        virtual const char* GetName()        const = 0;
        virtual const char* GetVersion()     const { return "1.0.0"; }
        virtual const char* GetDescription() const { return ""; }
        virtual const char* GetAuthor()      const { return "Unknown"; }

        bool IsEnabled() const { return m_Enabled; }
        void SetEnabled(bool e){ m_Enabled = e; }

    protected:
        bool m_Enabled{true};
    };

} // namespace Good