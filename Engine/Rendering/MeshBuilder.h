#pragma once
#include "../Core/Core.h"
#include "Mesh.h"

namespace Good
{

    class MeshBuilder
    {
    public:
        MeshBuilder();

        MeshBuilder& AddVertex(const Vertex& v);
        MeshBuilder& AddVertex(const Vec3& pos, const Vec3& normal, const Vec2& uv, const Color& color);
        MeshBuilder& AddTriangle(uint32 a, uint32 b, uint32 c);
        MeshBuilder& AddQuad(uint32 a, uint32 b, uint32 c, uint32 d);

        void     Clear();
        Ref<Mesh> Build();
        uint32   GetVertexCount() const { return static_cast<uint32>(m_Vertices.size()); }

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<uint32> m_Indices;
    };

} // namespace Good