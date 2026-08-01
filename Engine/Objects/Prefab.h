#pragma once
#include "../Core/Core.h"
#include "GameObject.h"

namespace Good
{

    class Prefab
    {
    public:
        Prefab() = default;
        explicit Prefab(const std::string& name);

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName()    const { return m_Name; }

        Ref<GameObject> Instantiate() const;
        Ref<GameObject> Instantiate(const Vec3& position) const;
        Ref<GameObject> Instantiate(const Vec3& position, const Quat& rotation) const;

        void SetSourceObject(Ref<GameObject> obj) { m_Source = std::move(obj); }
        Ref<GameObject> GetSourceObject()  const  { return m_Source; }

    private:
        std::string     m_Name{"Prefab"};
        Ref<GameObject> m_Source;
    };

} // namespace Good