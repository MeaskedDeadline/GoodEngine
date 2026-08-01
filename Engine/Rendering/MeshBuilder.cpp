#include "MeshBuilder.h"

namespace Good
{

    MeshBuilder::MeshBuilder() {}

    MeshBuilder& MeshBuilder::AddVertex(const Vertex& v) { m_Vertices.push_back(v); return *this; }

    MeshBuilder& MeshBuilder::AddVertex(const Vec3& p, const Vec3& n, const Vec2& uv, const Color& c)
    {
        m_Vertices.push_back(Vertex(p, n, uv, c));
        return *this;
    }

    MeshBuilder& MeshBuilder::AddTriangle(uint32 a, uint32 b, uint32 c)
    {
        m_Indices.push_back(a); m_Indices.push_back(b); m_Indices.push_back(c);
        return *this;
    }

    MeshBuilder& MeshBuilder::AddQuad(uint32 a, uint32 b, uint32 c, uint32 d)
    {
        AddTriangle(a, b, c);
        AddTriangle(c, d, a);
        return *this;
    }

    void MeshBuilder::Clear() { m_Vertices.clear(); m_Indices.clear(); }

    Ref<Mesh> MeshBuilder::Build()
    {
        auto mesh = MakeRef<Mesh>();
        mesh->Create(m_Vertices, m_Indices);
        Clear();
        return mesh;
    }

} // namespace Good