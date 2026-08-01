#include "Core/Engine.h"
#include "Core/App.h"
#include "Core/Log.h"
#include "Core/Time.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"
#include "Math/Vec3.h"
#include "Scene/Scene.h"

class GameApp final : public Good::App
{
public:
    void OnStart() override
    {
        GOOD_LOG_INFO("Game", "Game started!");
        m_Scene = Good::MakeRef<Good::Scene>("MainScene");
        auto cube = m_Scene->CreateNode("Cube");
        cube->GetLocalTransform().position = Good::Vec3(0, 0, 0);
        GOOD_LOG_INFO("Game", "Nodes: {}", m_Scene->GetNodeCount());
    }

    void OnUpdate() override
    {
        if (Good::Input::Get().IsKeyPressed(Good::Key::Escape))
        {
            GOOD_LOG_INFO("Game", "Quit");
            Good::Engine::Get().Quit();
        }

        if (Good::Input::Get().IsKeyPressed(Good::Key::Space))
            GOOD_LOG_INFO("Game", "FPS: {:.1f}", Good::Time::FPS());
    }

    void OnRender() override {}
    void OnUI()     override {}

    void OnStop() override
    {
        GOOD_LOG_INFO("Game", "Frames: {}", Good::Time::FrameCount());
    }

    const char* GetName() const override { return "GoodProject"; }

private:
    Good::Ref<Good::Scene> m_Scene;
};

Good::Scope<Good::App> Good::CreateApp()
{
    return Good::MakeScope<GameApp>();
}

int main()
{
    Good::EngineConfig config;
    config.appName      = "GoodProject";
    config.windowWidth  = 1280;
    config.windowHeight = 720;
    config.vsync        = true;

    if (!Good::Engine::Get().Initialize(config))
        return -1;

    Good::Engine::Get().Run(Good::CreateApp());
    return 0;
}