#pragma once
#include "../Core/Core.h"
#include "../Math/Transform.h"

namespace Good
{

    class Scene;

    class GameObject
    {
    public:
        GameObject();
        explicit GameObject(const std::string& name);
        ~GameObject();

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        uint64 GetID()    const { return m_ID;    }
        bool   IsActive() const { return m_Active; }
        void   SetActive(bool v){ m_Active = v;    }

        Transform&       GetTransform()       { return m_Transform; }
        const Transform& GetTransform() const { return m_Transform; }

        GameObject* GetParent()  const { return m_Parent; }
        void SetParent(GameObject* p)  { m_Parent = p; }

        void AddChild(Ref<GameObject> child);
        void RemoveChild(Ref<GameObject> child);
        const std::vector<Ref<GameObject>>& GetChildren() const { return m_Children; }

        void OnStart();
        void OnUpdate(float32 dt);
        void OnFixedUpdate(float32 dt);
        void OnDestroy();

        bool IsDestroyed() const { return m_Destroyed; }
        void Destroy()           { m_Destroyed = true; }

        static uint64 GenerateID();

    private:
        uint64                        m_ID{0};
        std::string                   m_Name{"GameObject"};
        Transform                     m_Transform;
        bool                          m_Active{true};
        bool                          m_Destroyed{false};
        GameObject*                   m_Parent{nullptr};
        std::vector<Ref<GameObject>>  m_Children;
    };

} // namespace Good