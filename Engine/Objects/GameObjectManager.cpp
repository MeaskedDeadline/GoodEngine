#include "GameObjectManager.h"
#include "../Core/Log.h"

namespace Good
{

    GameObjectManager& GameObjectManager::Get()
    {
        static GameObjectManager s_Instance;
        return s_Instance;
    }

    Ref<GameObject> GameObjectManager::Create(const std::string& name)
    {
        auto obj = MakeRef<GameObject>(name);
        m_Objects.push_back(obj);
        obj->OnStart();
        return obj;
    }

    Ref<GameObject> GameObjectManager::Find(const std::string& name) const
    {
        for (auto& o : m_Objects)
            if (o->GetName() == name) return o;
        return nullptr;
    }

    Ref<GameObject> GameObjectManager::FindByID(uint64 id) const
    {
        for (auto& o : m_Objects)
            if (o->GetID() == id) return o;
        return nullptr;
    }

    void GameObjectManager::Destroy(Ref<GameObject> obj)
    {
        if (obj) obj->Destroy();
    }

    void GameObjectManager::Update(float32 dt)
    {
        for (auto& o : m_Objects)
            if (o->IsActive() && !o->IsDestroyed())
                o->OnUpdate(dt);
        FlushDestroyed();
    }

    void GameObjectManager::FixedUpdate(float32 dt)
    {
        for (auto& o : m_Objects)
            if (o->IsActive() && !o->IsDestroyed())
                o->OnFixedUpdate(dt);
    }

    void GameObjectManager::FlushDestroyed()
    {
        m_Objects.erase(
            std::remove_if(m_Objects.begin(), m_Objects.end(),
                [](const Ref<GameObject>& o) { return o->IsDestroyed(); }),
            m_Objects.end());
    }

    void GameObjectManager::Clear() { m_Objects.clear(); }

} // namespace Good