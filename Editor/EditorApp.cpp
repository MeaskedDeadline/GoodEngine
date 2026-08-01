#include "EditorApp.h"
#include "Platform/Window.h"
#include "Core/Log.h"
#include "Core/Time.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"
#include "Debug/Profiler.h"
#include "Debug/DebugDraw.h"

namespace Good
{

void EditorApp::OnStart()
{
    GOOD_LOG_INFO("Editor", "Starting GoodEngine Editor...");

    Profiler::Get().Enable();

    uint32 w = Window::Get().Width();
    uint32 h = Window::Get().Height();

    if (!m_EditorLayer.Initialize(w, h))
    {
        GOOD_LOG_FATAL("Editor", "Failed to initialize editor!");
        Engine::Get().Quit();
        return;
    }

    m_EditorLayer.Resize(w, h);
    m_EditorLayer.Update(0.016f);

    // Принудительный первый рендер
    Window::Get().BeginFrame();
    m_EditorLayer.GetMainWindow().GetViewport().RenderScene();
    m_EditorLayer.Render();
    Window::Get().EndFrame();

    GOOD_LOG_INFO("Editor", "Editor ready! Size: {}x{}", w, h);
}

void EditorApp::OnUpdate()
{
    Profiler::Get().BeginSample("Update");

    if (Input::Get().IsKeyPressed(Key::Escape))
    {
        GOOD_LOG_INFO("Editor", "Escape pressed");
        Engine::Get().Quit();
        Profiler::Get().EndSample("Update");
        return;
    }

    static uint32 lastW = 0, lastH = 0;
    uint32 w = Window::Get().Width();
    uint32 h = Window::Get().Height();
    if (w != lastW || h != lastH)
    {
        m_EditorLayer.Resize(w, h);
        lastW = w;
        lastH = h;
    }

    m_EditorLayer.Update(Time::DeltaTime());

    Profiler::Get().EndSample("Update");
    Profiler::Get().BeginFrame();
}

void EditorApp::OnRender()
{
    Profiler::Get().BeginSample("Render");
    m_EditorLayer.GetMainWindow().GetViewport().RenderScene();
    Profiler::Get().EndSample("Render");
}

void EditorApp::OnUI()
{
    Profiler::Get().BeginSample("UI");
    m_EditorLayer.Render();
    DebugDraw::Update(Time::DeltaTime());
    Profiler::Get().EndSample("UI");
    Profiler::Get().EndFrame();
}

void EditorApp::OnStop()
{
    GOOD_LOG_INFO("Editor", "Editor stopping...");
    m_EditorLayer.Shutdown();
    GOOD_LOG_INFO("Editor", "Frames: {}", Time::FrameCount());
}

void EditorApp::OnFixedUpdate() {}

Scope<App> CreateApp()
{
    return MakeScope<EditorApp>();
}

int main()
{
    EngineConfig config;
    config.appName      = "GoodEngine Editor";
    config.windowWidth  = 1920;
    config.windowHeight = 1080;
    config.vsync        = true;
    config.logFilePath  = "Logs/Editor.log";

    if (!Engine::Get().Initialize(config))
        return -1;

    Engine::Get().Run(CreateApp());
    return 0;
}

} // namespace Good