#include "SceneManager.h"
#include "../Core/Log.h"

namespace Good
{

    SceneManager& SceneManager::Get()
    {
        static SceneManager s_Instance;
        return s_Instance;
    }

    Ref<Scene> SceneManager::CreateScene(const std::string& name)
    {
        auto scene = MakeRef<Scene>(name);
        m_Scenes.push_back(scene);
        if (!m_ActiveScene) m_ActiveScene = scene;
        GOOD_LOG_INFO("SceneManager", "Scene created: '{}'", name);
        return scene;
    }

    Ref<Scene> SceneManager::LoadScene(const std::string& path)
    {
        GOOD_LOG_INFO("SceneManager", "Loading scene: '{}'", path);
        auto scene = MakeRef<Scene>(path);
        m_Scenes.push_back(scene);
        m_ActiveScene = scene;
        return scene;
    }

    void SceneManager::SetActiveScene(Ref<Scene> scene)
    {
        m_ActiveScene = std::move(scene);
    }

    void SceneManager::Update(float32 dt)
    {
        if (m_ActiveScene) m_ActiveScene->Update(dt);
    }

    void SceneManager::Clear()
    {
        m_Scenes.clear();
        m_ActiveScene = nullptr;
    }

} // namespace Good