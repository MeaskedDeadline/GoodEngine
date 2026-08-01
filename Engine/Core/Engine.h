#pragma once
#include "Core.h"
#include "Log.h"

namespace Good
{

    class App;

    struct EngineConfig
    {
        std::string appName          = "GoodEngine";
        uint32      windowWidth      = 1280;
        uint32      windowHeight     = 720;
        bool        fullscreen       = false;
        bool        vsync            = true;
        std::string logFilePath      = "GoodEngine.log";
    };

    class Engine : public NonCopyable
    {
    public:
        static Engine& Get();

        bool Initialize(const EngineConfig& config);
        void Run(Scope<App> app);
        void Quit();

        bool               IsRunning() const { return m_Running;  }
        const EngineConfig& Config()   const { return m_Config;   }
        static const char* VersionString()   { return "1.0.0";    }

    private:
        Engine() = default;
        void Shutdown();

        EngineConfig m_Config;
        bool         m_Running{false};
        bool         m_Initialized{false};
    };

} // namespace Good