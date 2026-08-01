#pragma once
#include "Component.h"
#include "../Rendering/Mesh.h"
#include "../Graphics/Material.h"

namespace Good
{

    class MeshComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "MeshComponent"; }

        void SetMesh(Ref<Mesh> mesh)         { m_Mesh = std::move(mesh); }
        void SetMaterial(Ref<Material> mat)  { m_Material = std::move(mat); }

        Ref<Mesh>     GetMesh()     const { return m_Mesh;     }
        Ref<Material> GetMaterial() const { return m_Material; }

        bool CastShadows()    const { return m_CastShadows;    }
        bool ReceiveShadows() const { return m_ReceiveShadows; }

        void SetCastShadows(bool v)    { m_CastShadows = v;    }
        void SetReceiveShadows(bool v) { m_ReceiveShadows = v; }

    private:
        Ref<Mesh>     m_Mesh;
        Ref<Material> m_Material;
        bool          m_CastShadows{true};
        bool          m_ReceiveShadows{true};
    };

} // namespace Good