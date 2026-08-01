#include "Engine.h"
#include "App.h"
#include "Loop.h"
#include "Assert.h"
#include "Time.h"
#include "../Platform/Window.h"
#include "../Input/Input.h"
#include "../Graphics/GraphicsContext.h"

namespace Good
{

    Engine& Engine::Get()
    {
        static Engine s_Instance;
        return s_Instance;
    }

    bool Engine::Initialize(const EngineConfig& config)
    {
        GOOD_ASSERT(!m_Initialized, "Engine already initialized");
        m_Config = config;

        if (!config.logFilePath.empty())
            Logger::Get().AddSink(MakeRef<FileSink>(config.logFilePath));

        GOOD_LOG_INFO("Engine", "GoodEngine v{} starting...", VersionString());

        WindowConfig wc;
        wc.title      = config.appName;
        wc.width      = config.windowWidth;
        wc.height     = config.windowHeight;
        wc.fullscreen = config.fullscreen;
        wc.vsync      = config.vsync;

        if (!Window::Get().Initialize(wc))
        {
            GOOD_LOG_FATAL("Engine", "Window init failed!");
            return false;
        }

        Input::Get().Initialize();

        if (!GraphicsContext::Get().Initialize())
        {
            GOOD_LOG_FATAL("Engine", "Graphics init failed!");
            return false;
        }

        Time::Initialize();

        m_Initialized = true;
        m_Running     = true;
        GOOD_LOG_INFO("Engine", "Engine ready.");
        return true;
    }

    void Engine::Run(Scope<App> app)
    {
        GOOD_ASSERT(m_Initialized, "Engine not initialized");
        GOOD_ASSERT(app != nullptr, "App is null");
        Loop loop;
        loop.Run(std::move(app));
        Shutdown();
    }

    void Engine::Quit() { m_Running = false; }

    void Engine::Shutdown()
    {
        GOOD_LOG_INFO("Engine", "Shutting down...");
        GraphicsContext::Get().Shutdown();
        Input::Get().Shutdown();
        Window::Get().Shutdown();
        m_Running     = false;
        m_Initialized = false;
        GOOD_LOG_INFO("Engine", "Goodbye!");
    }

} // namespace Good