#pragma once
#include "../Core/Core.h"
#include "GameObject.h"

namespace Good
{

    class GameObjectManager : public NonCopyable
    {
    public:
        static GameObjectManager& Get();

        Ref<GameObject> Create(const std::string& name = "GameObject");
        Ref<GameObject> Find(const std::string& name) const;
        Ref<GameObject> FindByID(uint64 id) const;

        void Destroy(Ref<GameObject> obj);
        void Update(float32 dt);
        void FixedUpdate(float32 dt);
        void FlushDestroyed();
        void Clear();

        usize GetCount() const { return m_Objects.size(); }
        const std::vector<Ref<GameObject>>& GetAll() const { return m_Objects; }

    private:
        GameObjectManager() = default;

        std::vector<Ref<GameObject>> m_Objects;
    };

} // namespace Good