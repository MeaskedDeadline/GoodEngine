#pragma once
#include "../Core/Core.h"
#include "../Math/Vec2.h"
#include "../Math/Vec3.h"
#include "../Math/Color.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/VertexArray.h"

namespace Good
{

    struct Vertex
    {
        Vec3  position{0,0,0};
        Vec3  normal{0,1,0};
        Vec2  texCoord{0,0};
        Color color{1,1,1,1};

        Vertex() = default;
        Vertex(const Vec3& p, const Vec3& n, const Vec2& uv, const Color& c)
            : position(p), normal(n), texCoord(uv), color(c) {}
    };

    class Mesh
    {
    public:
        Mesh()  = default;
        ~Mesh() = default;

        void Create(const std::vector<Vertex>& verts, const std::vector<uint32>& indices);
        void Bind()   const;
        void Unbind() const;
        void Draw()   const;

        uint32 GetVertexCount() const { return m_VertexCount; }
        uint32 GetIndexCount()  const { return m_IndexCount;  }

        static Ref<Mesh> CreateQuad();
        static Ref<Mesh> CreateCube();
        static Ref<Mesh> CreateSphere(uint32 stacks = 16, uint32 slices = 16);
        static Ref<Mesh> CreatePlane(float32 size = 10.0f, uint32 sub = 1);
        static Ref<Mesh> CreateCapsule(float32 radius = 0.5f, float32 height = 2.0f);

    private:
        VertexArray  m_VAO;
        VertexBuffer m_VBO;
        IndexBuffer  m_IBO;
        uint32       m_VertexCount{0};
        uint32       m_IndexCount{0};
    };

} // namespace Good