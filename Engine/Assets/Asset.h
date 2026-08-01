#pragma once
#include "../Core/Core.h"
#include "../Utilities/UUID.h"

namespace Good
{

    enum class AssetType : uint16
    {
        Unknown = 0,
        Texture,
        Mesh,
        Shader,
        Material,
        AudioClip,
        AnimationClip,
        Scene,
        Prefab,
        Font,
        Script,
        Count
    };

    const char* AssetTypeToString(AssetType type);
    AssetType   AssetTypeFromExtension(const std::string& ext);

    enum class AssetState : uint8 { Unloaded, Loading, Ready, Failed };

    class Asset
    {
    public:
        Asset() : m_UUID(UUID::Generate()) {}
        virtual ~Asset() = default;

        const UUID&        GetUUID()     const { return m_UUID;     }
        AssetType          GetType()     const { return m_Type;     }
        AssetState         GetState()    const { return m_State;    }
        const std::string& GetPath()     const { return m_Path;     }
        const std::string& GetName()     const { return m_Name;     }
        usize              GetMemSize()  const { return m_MemSize;  }
        bool               IsReady()     const { return m_State == AssetState::Ready; }

        void SetPath(const std::string& p)    { m_Path  = p; }
        void SetName(const std::string& n)    { m_Name  = n; }
        void SetState(AssetState s)           { m_State = s; }
        void SetMemSize(usize sz)             { m_MemSize = sz; }

    protected:
        UUID        m_UUID;
        AssetType   m_Type{AssetType::Unknown};
        AssetState  m_State{AssetState::Unloaded};
        std::string m_Path;
        std::string m_Name;
        usize       m_MemSize{0};
    };

} // namespace Good