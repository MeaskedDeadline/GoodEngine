#include "EditorLayer.h"
#include "UI/UIContext.h"
#include "UI/UIInput.h"
#include "UI/FontAtlas.h"
#include "UI/IconAtlas.h"
#include "UI/ImageLoader.h"
#include "Debug/DebugConsole.h"
#include "Core/Log.h"
#include "Core/Time.h"
#include "Input/Input.h"
#include "Input/MouseCodes.h"
#include "Themes/DarkTheme.h"
#include "Serialization/SceneSerializer.h"

namespace Good
{

EditorLayer::EditorLayer()  = default;
EditorLayer::~EditorLayer() { Shutdown(); }

bool EditorLayer::Initialize(uint32 w, uint32 h)
{
    DarkTheme theme;
    UIContext::Get().GetStyle() = theme.GetStyle();

    if (!UIContext::Get().Initialize(w, h))
    {
        GOOD_LOG_ERROR("Editor", "UIContext init failed");
        return false;
    }

    // Шрифт
    bool fontLoaded = false;
    if (!fontLoaded) fontLoaded = FontAtlas::Get().LoadTTF("C:/Users/Lenovo/Desktop/GoodEngine/Editor/Assets/Font.ttf", 20.0f);
    if (!fontLoaded) fontLoaded = FontAtlas::Get().LoadTTF("C:/Windows/Fonts/consola.ttf", 18.0f);
    if (!fontLoaded) fontLoaded = FontAtlas::Get().LoadTTF("C:/Windows/Fonts/arial.ttf", 18.0f);
    if (!fontLoaded) { FontAtlas::Get().Initialize(); }

    // Иконки — сначала пробуем загрузить атлас из файла
    if (!IconAtlas::Get().Load("C:/Users/Lenovo/Desktop/GoodEngine/Editor/Assets/icons.png", 32, 32))
    {
        // Если PNG не загрузился — пробуем BMP
        if (!IconAtlas::Get().Load("C:/Users/Lenovo/Desktop/GoodEngine/Editor/Assets/icons.bmp", 32, 32))
        {
            // Если нет файлов — генерируем программно
            GOOD_LOG_WARN("Editor", "Icon atlas not found, generating default");
            IconAtlas::Get().CreateDefault();
        }
    }

    if (!m_MainWindow.Initialize(w, h))
    {
        GOOD_LOG_ERROR("Editor", "MainWindow init failed");
        return false;
    }

    // File menu callbacks
    m_MainWindow.m_OnNewScene = [this]()
    {
        GOOD_LOG_INFO("Editor", "New Scene");
        m_ActiveScene->Clear();
        m_ActiveScene = SceneManager::Get().CreateScene("New Scene");
        m_MainWindow.GetHierarchy().SetScene(m_ActiveScene);
        m_MainWindow.GetViewport().SetScene(m_ActiveScene);
    };

    m_MainWindow.m_OnSaveScene = [this]()
    {
        if (m_ActiveScene)
        {
            std::string path = "C:/Users/Lenovo/Desktop/GoodEngine/scene.json";
            SceneSerializer::SaveToFile(*m_ActiveScene, path);
            GOOD_LOG_INFO("Editor", "Scene saved to '{}'", path);
        }
    };

    m_MainWindow.m_OnLoadScene = [this]()
    {
        if (m_ActiveScene)
        {
            std::string path = "C:/Users/Lenovo/Desktop/GoodEngine/scene.json";
            if (SceneSerializer::LoadFromFile(*m_ActiveScene, path))
            {
                m_MainWindow.GetHierarchy().SetScene(m_ActiveScene);
                m_MainWindow.GetViewport().SetScene(m_ActiveScene);
                GOOD_LOG_INFO("Editor", "Scene loaded from '{}'", path);
            }
            else
            {
                GOOD_LOG_ERROR("Editor", "Failed to load scene");
            }
        }
    };

    SetupDefaultScene();
    SetupDebugConsole();

    GOOD_LOG_INFO("Editor", "EditorLayer initialized");
    return true;
}

void EditorLayer::Shutdown()
{
    IconAtlas::Get().Shutdown();
    FontAtlas::Get().Shutdown();
    UIContext::Get().Shutdown();
    SceneManager::Get().Clear();
    GOOD_LOG_INFO("Editor", "EditorLayer shutdown");
}

void EditorLayer::SetupDefaultScene()
{
    m_ActiveScene = SceneManager::Get().CreateScene("MainScene");

    auto root = m_ActiveScene->CreateNode("Scene Root");

    auto floor = m_ActiveScene->CreateNode("Floor", root);
    floor->SetType(NodeType::Plane);
    floor->SetColor(Color(0.4f, 0.4f, 0.45f, 1.0f));
    floor->GetLocalTransform().scale = Vec3(15, 1, 15);

    auto cube = m_ActiveScene->CreateNode("Cube", root);
    cube->SetType(NodeType::Cube);
    cube->SetColor(Color(0.85f, 0.3f, 0.3f, 1.0f));
    cube->GetLocalTransform().position = Vec3(-2, 0.5f, 0);

    auto sphere = m_ActiveScene->CreateNode("Sphere", root);
    sphere->SetType(NodeType::Sphere);
    sphere->SetColor(Color(0.3f, 0.6f, 0.9f, 1.0f));
    sphere->GetLocalTransform().position = Vec3(2, 0.5f, 0);

    auto capsule = m_ActiveScene->CreateNode("Capsule", root);
    capsule->SetType(NodeType::Capsule);
    capsule->SetColor(Color(0.3f, 0.85f, 0.4f, 1.0f));
    capsule->GetLocalTransform().position = Vec3(0, 1.0f, -2);

    auto cube2 = m_ActiveScene->CreateNode("Cube2", root);
    cube2->SetType(NodeType::Cube);
    cube2->SetColor(Color(0.95f, 0.85f, 0.3f, 1.0f));
    cube2->GetLocalTransform().position = Vec3(0, 0.5f, 2);
    cube2->GetLocalTransform().scale    = Vec3(0.7f, 0.7f, 0.7f);

    auto light = m_ActiveScene->CreateNode("Directional Light", root);
    light->SetType(NodeType::DirectionalLight);
    light->GetLocalTransform().position = Vec3(3, 5, 3);
    light->GetLocalTransform().rotation = Quat::FromEuler(-45, 45, 0);

    auto camera = m_ActiveScene->CreateNode("Main Camera", root);
    camera->SetType(NodeType::Camera);
    camera->GetLocalTransform().position = Vec3(0, 2, 8);

    m_MainWindow.GetHierarchy().SetScene(m_ActiveScene);
    m_MainWindow.GetViewport().SetScene(m_ActiveScene);

    GOOD_LOG_INFO("Editor", "Default scene created with {} objects",
        m_ActiveScene->GetNodeCount());
}

void EditorLayer::SetupDebugConsole()
{
    auto& console = DebugConsole::Get();

    struct NullDeleter { void operator()(ILogSink*) const {} };
    Logger::Get().AddSink(std::shared_ptr<ILogSink>(&console, NullDeleter{}));

    console.RegisterCommand("help", [](const std::vector<std::string>&)
    {
        GOOD_LOG_INFO("Console", "Commands: help, clear, scene.info, time.info, font.load");
    });

    console.RegisterCommand("clear", [](const std::vector<std::string>&)
    {
        DebugConsole::Get().Clear();
    });

    console.RegisterCommand("scene.info", [](const std::vector<std::string>&)
    {
        auto scene = SceneManager::Get().GetActiveScene();
        if (scene)
            GOOD_LOG_INFO("Console", "Scene: '{}' Nodes: {}",
                scene->GetName(), scene->GetNodeCount());
    });

    console.RegisterCommand("time.info", [](const std::vector<std::string>&)
    {
        GOOD_LOG_INFO("Console", "FPS: {} DT: {}ms",
            static_cast<int32>(Time::FPS()),
            static_cast<int32>(Time::DeltaTime() * 1000));
    });

    console.RegisterCommand("font.load", [](const std::vector<std::string>& args)
    {
        if (args.empty()) { GOOD_LOG_WARN("Console", "Usage: font.load path [size]"); return; }
        float32 size = args.size() >= 2 ? std::stof(args[1]) : 20.0f;
        FontAtlas::Get().LoadTTF(args[0], size);
    });
}

void EditorLayer::Update(float32 dt)
{
    auto& input = Input::Get();

    UIInput::Get().OnMouseMove(input.GetMouseX(), input.GetMouseY());
    UIInput::Get().OnMouseButton(0, input.IsMouseButtonDown(MouseButton::Left));
    UIInput::Get().OnMouseButton(1, input.IsMouseButtonDown(MouseButton::Right));
    UIInput::Get().OnMouseButton(2, input.IsMouseButtonDown(MouseButton::Middle));
    UIInput::Get().OnMouseScroll(input.GetScrollX(), input.GetScrollY());

    m_MainWindow.OnUpdate(dt);

    if (m_ActiveScene)
        m_ActiveScene->Update(dt);
}

void EditorLayer::Render()
{
    m_MainWindow.OnRender();
    UIInput::Get().EndFrame();
}

void EditorLayer::Resize(uint32 w, uint32 h)
{
    UIContext::Get().Resize(w, h);
    m_MainWindow.Resize(w, h);
}

} // namespace Good