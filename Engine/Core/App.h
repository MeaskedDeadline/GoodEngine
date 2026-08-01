#pragma once
#include "Core.h"

namespace Good
{

    class App
    {
    public:
        virtual ~App() = default;
        virtual void OnStart()       {}
        virtual void OnUpdate()      {}
        virtual void OnRender()      {}
        virtual void OnUI()          {}
        virtual void OnStop()        {}
        virtual void OnFixedUpdate() {}
        virtual const char* GetName() const { return "GoodApp"; }
    };

    extern Scope<App> CreateApp();

} // namespace Good