#include "Prefab.h"
#include "GameObjectManager.h"

namespace Good
{

    Prefab::Prefab(const std::string& name) : m_Name(name) {}

    Ref<GameObject> Prefab::Instantiate() const
    {
        return GameObjectManager::Get().Create(m_Name + "_instance");
    }

    Ref<GameObject> Prefab::Instantiate(const Vec3& position) const
    {
        auto obj = GameObjectManager::Get().Create(m_Name + "_instance");
        obj->GetTransform().position = position;
        return obj;
    }

    Ref<GameObject> Prefab::Instantiate(const Vec3& position, const Quat& rotation) const
    {
        auto obj = GameObjectManager::Get().Create(m_Name + "_instance");
        obj->GetTransform().position = position;
        obj->GetTransform().rotation = rotation;
        return obj;
    }

} // namespace Good