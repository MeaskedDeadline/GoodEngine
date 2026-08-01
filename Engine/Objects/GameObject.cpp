#include "GameObject.h"
#include "../Core/Log.h"
#include <atomic>

namespace Good
{

    static std::atomic<uint64> s_NextID{1};

    uint64 GameObject::GenerateID() { return s_NextID++; }

    GameObject::GameObject()
        : m_ID(GenerateID()) {}

    GameObject::GameObject(const std::string& name)
        : m_ID(GenerateID()), m_Name(name) {}

    GameObject::~GameObject() {}

    void GameObject::AddChild(Ref<GameObject> child)
    {
        if (!child) return;
        child->m_Parent = this;
        m_Children.push_back(std::move(child));
    }

    void GameObject::RemoveChild(Ref<GameObject> child)
    {
        if (!child) return;
        for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
        {
            if (*it == child)
            {
                (*it)->m_Parent = nullptr;
                m_Children.erase(it);
                return;
            }
        }
    }

    void GameObject::OnStart()              {}
    void GameObject::OnUpdate(float32)      {}
    void GameObject::OnFixedUpdate(float32) {}
    void GameObject::OnDestroy()            {}

} // namespace Good