#pragma once
#include "../Core/Core.h"
#include "../Utilities/UUID.h"

namespace Good
{

    template<typename T>
    class ResourceHandle
    {
    public:
        ResourceHandle() = default;
        explicit ResourceHandle(Ref<T> resource) : m_Resource(std::move(resource)) {}
        explicit ResourceHandle(UUID uuid) : m_UUID(uuid) {}

        T*          Get()       const { return m_Resource.get(); }
        Ref<T>      GetRef()    const { return m_Resource; }
        const UUID& GetUUID()   const { return m_UUID; }

        bool IsValid() const { return m_Resource != nullptr; }
        bool IsNull()  const { return !IsValid(); }

        T* operator->() const { return m_Resource.get(); }
        T& operator*()  const { return *m_Resource; }

        explicit operator bool() const { return IsValid(); }

    private:
        Ref<T> m_Resource;
        UUID   m_UUID{UUID::Invalid()};
    };

} // namespace Good