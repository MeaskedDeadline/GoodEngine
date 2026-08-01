#include "Scene.h"
#include "../Core/Log.h"

namespace Good
{

    Scene::Scene()  { GOOD_LOG_INFO("Scene", "Created: '{}'", m_Name); }
    Scene::Scene(const std::string& name) : m_Name(name) { GOOD_LOG_INFO("Scene", "Created: '{}'", m_Name); }
    Scene::~Scene() { GOOD_LOG_INFO("Scene", "Destroyed: '{}'", m_Name); }

    Ref<SceneNode> Scene::CreateNode(const std::string& name) { return m_Graph.CreateNode(name); }
    Ref<SceneNode> Scene::CreateNode(const std::string& name, Ref<SceneNode> parent) { return m_Graph.CreateNode(name, parent); }
    Ref<SceneNode> Scene::FindNode(const std::string& name) const { return m_Graph.Find(name); }
    void Scene::Update(float32) {}
    void Scene::Clear() { m_Graph.Clear(); }

} // namespace Good