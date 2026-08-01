#include "Mesh.h"
#include "../Graphics/GLFunctions.h"
#include "../Math/MathCommon.h"
#include <cstddef>

namespace Good
{

void Mesh::Create(const std::vector<Vertex>& v, const std::vector<uint32>& idx)
{
    m_VertexCount = static_cast<uint32>(v.size());
    m_IndexCount  = static_cast<uint32>(idx.size());

    m_VAO.Create();
    m_VAO.Bind();
    m_VBO.Create(v.data(), v.size() * sizeof(Vertex));
    m_IBO.Create(idx.data(), m_IndexCount);

    int32 stride = static_cast<int32>(sizeof(Vertex));
    m_VAO.SetAttribute(0, 3, GL_FLOAT, false, stride, offsetof(Vertex, position));
    m_VAO.SetAttribute(1, 3, GL_FLOAT, false, stride, offsetof(Vertex, normal));
    m_VAO.SetAttribute(2, 2, GL_FLOAT, false, stride, offsetof(Vertex, texCoord));
    m_VAO.SetAttribute(3, 4, GL_FLOAT, false, stride, offsetof(Vertex, color));
    m_VAO.Unbind();
}

void Mesh::Bind()   const { m_VAO.Bind(); }
void Mesh::Unbind() const { m_VAO.Unbind(); }

void Mesh::Draw() const
{
    m_VAO.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_IndexCount), GL_UNSIGNED_INT, nullptr);
}

Ref<Mesh> Mesh::CreateQuad()
{
    Color w = Color::White();
    std::vector<Vertex> v;
    v.push_back(Vertex(Vec3(-0.5f,-0.5f,0), Vec3(0,0,1), Vec2(0,0), w));
    v.push_back(Vertex(Vec3( 0.5f,-0.5f,0), Vec3(0,0,1), Vec2(1,0), w));
    v.push_back(Vertex(Vec3( 0.5f, 0.5f,0), Vec3(0,0,1), Vec2(1,1), w));
    v.push_back(Vertex(Vec3(-0.5f, 0.5f,0), Vec3(0,0,1), Vec2(0,1), w));
    std::vector<uint32> i = {0,1,2,2,3,0};
    auto m = MakeRef<Mesh>(); m->Create(v,i); return m;
}

Ref<Mesh> Mesh::CreateCube()
{
    Color w = Color::White();
    std::vector<Vertex> v;

    auto face = [&](Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 n)
    {
        v.push_back(Vertex(a, n, Vec2(0,0), w));
        v.push_back(Vertex(b, n, Vec2(1,0), w));
        v.push_back(Vertex(c, n, Vec2(1,1), w));
        v.push_back(Vertex(d, n, Vec2(0,1), w));
    };

    face(Vec3(-0.5f,-0.5f, 0.5f), Vec3( 0.5f,-0.5f, 0.5f), Vec3( 0.5f, 0.5f, 0.5f), Vec3(-0.5f, 0.5f, 0.5f), Vec3(0,0,1));
    face(Vec3( 0.5f,-0.5f,-0.5f), Vec3(-0.5f,-0.5f,-0.5f), Vec3(-0.5f, 0.5f,-0.5f), Vec3( 0.5f, 0.5f,-0.5f), Vec3(0,0,-1));
    face(Vec3(-0.5f, 0.5f, 0.5f), Vec3( 0.5f, 0.5f, 0.5f), Vec3( 0.5f, 0.5f,-0.5f), Vec3(-0.5f, 0.5f,-0.5f), Vec3(0,1,0));
    face(Vec3(-0.5f,-0.5f,-0.5f), Vec3( 0.5f,-0.5f,-0.5f), Vec3( 0.5f,-0.5f, 0.5f), Vec3(-0.5f,-0.5f, 0.5f), Vec3(0,-1,0));
    face(Vec3( 0.5f,-0.5f, 0.5f), Vec3( 0.5f,-0.5f,-0.5f), Vec3( 0.5f, 0.5f,-0.5f), Vec3( 0.5f, 0.5f, 0.5f), Vec3(1,0,0));
    face(Vec3(-0.5f,-0.5f,-0.5f), Vec3(-0.5f,-0.5f, 0.5f), Vec3(-0.5f, 0.5f, 0.5f), Vec3(-0.5f, 0.5f,-0.5f), Vec3(-1,0,0));

    std::vector<uint32> idx;
    for (uint32 f = 0; f < 6; ++f)
    {
        uint32 b = f*4;
        idx.push_back(b); idx.push_back(b+1); idx.push_back(b+2);
        idx.push_back(b+2); idx.push_back(b+3); idx.push_back(b);
    }

    auto m = MakeRef<Mesh>(); m->Create(v, idx); return m;
}

Ref<Mesh> Mesh::CreateSphere(uint32 stacks, uint32 slices)
{
    Color w = Color::White();
    std::vector<Vertex> v;
    std::vector<uint32> idx;

    for (uint32 i = 0; i <= stacks; ++i)
    {
        float32 phi = Math::PI * static_cast<float32>(i) / static_cast<float32>(stacks);
        for (uint32 j = 0; j <= slices; ++j)
        {
            float32 theta = Math::TWO_PI * static_cast<float32>(j) / static_cast<float32>(slices);
            Vec3 pos(Math::Sin(phi)*Math::Cos(theta),
                     Math::Cos(phi),
                     Math::Sin(phi)*Math::Sin(theta));
            Vec2 uv(static_cast<float32>(j)/static_cast<float32>(slices),
                    static_cast<float32>(i)/static_cast<float32>(stacks));
            v.push_back(Vertex(pos, pos.Normalized(), uv, w));
        }
    }

    for (uint32 i = 0; i < stacks; ++i)
    {
        for (uint32 j = 0; j < slices; ++j)
        {
            uint32 a = i*(slices+1)+j, b = a+1;
            uint32 c = (i+1)*(slices+1)+j, d = c+1;
            idx.push_back(a); idx.push_back(c); idx.push_back(b);
            idx.push_back(b); idx.push_back(c); idx.push_back(d);
        }
    }

    auto m = MakeRef<Mesh>(); m->Create(v, idx); return m;
}

Ref<Mesh> Mesh::CreatePlane(float32 size, uint32 sub)
{
    Color w = Color::White();
    std::vector<Vertex> v;
    std::vector<uint32> idx;
    float32 half = size*0.5f, step = size/static_cast<float32>(sub);

    for (uint32 z = 0; z <= sub; ++z)
        for (uint32 x = 0; x <= sub; ++x)
        {
            float32 px = -half + static_cast<float32>(x)*step;
            float32 pz = -half + static_cast<float32>(z)*step;
            float32 u  = static_cast<float32>(x)/static_cast<float32>(sub);
            float32 vt = static_cast<float32>(z)/static_cast<float32>(sub);
            v.push_back(Vertex(Vec3(px,0,pz), Vec3(0,1,0), Vec2(u,vt), w));
        }

    for (uint32 z = 0; z < sub; ++z)
        for (uint32 x = 0; x < sub; ++x)
        {
            uint32 tl=z*(sub+1)+x, tr=tl+1, bl=(z+1)*(sub+1)+x, br=bl+1;
            idx.push_back(tl); idx.push_back(bl); idx.push_back(tr);
            idx.push_back(tr); idx.push_back(bl); idx.push_back(br);
        }

    auto m = MakeRef<Mesh>(); m->Create(v, idx); return m;
}

Ref<Mesh> Mesh::CreateCapsule(float32 radius, float32 height)
{
    uint32 stacks = 8;
    uint32 slices = 16;
    Color w = Color::White();
    std::vector<Vertex> v;
    std::vector<uint32> idx;

    float32 halfH = (height - radius * 2.0f) * 0.5f;
    if (halfH < 0) halfH = 0;

    // Верхняя полусфера
    for (uint32 i = 0; i <= stacks; ++i)
    {
        float32 phi = Math::HALF_PI * static_cast<float32>(i) / static_cast<float32>(stacks);
        float32 y   = Math::Sin(phi) * radius + halfH;
        float32 r   = Math::Cos(phi) * radius;

        for (uint32 j = 0; j <= slices; ++j)
        {
            float32 theta = Math::TWO_PI * static_cast<float32>(j) / static_cast<float32>(slices);
            Vec3 pos(r * Math::Cos(theta), y, r * Math::Sin(theta));
            Vec3 norm = Vec3(Math::Cos(phi) * Math::Cos(theta),
                             Math::Sin(phi),
                             Math::Cos(phi) * Math::Sin(theta));
            float32 u = static_cast<float32>(j) / static_cast<float32>(slices);
            float32 vt = 0.5f + static_cast<float32>(i) / static_cast<float32>(stacks * 2);
            v.push_back(Vertex(pos, norm.Normalized(), Vec2(u, vt), w));
        }
    }

    // Цилиндр (середина)
    for (uint32 i = 0; i <= 1; ++i)
    {
        float32 y = halfH - static_cast<float32>(i) * halfH * 2.0f;

        for (uint32 j = 0; j <= slices; ++j)
        {
            float32 theta = Math::TWO_PI * static_cast<float32>(j) / static_cast<float32>(slices);
            Vec3 pos(radius * Math::Cos(theta), y, radius * Math::Sin(theta));
            Vec3 norm(Math::Cos(theta), 0, Math::Sin(theta));
            float32 u = static_cast<float32>(j) / static_cast<float32>(slices);
            float32 vt = 0.5f - static_cast<float32>(i) * 0.0f;
            v.push_back(Vertex(pos, norm, Vec2(u, vt), w));
        }
    }

    // Нижняя полусфера
    for (uint32 i = 0; i <= stacks; ++i)
    {
        float32 phi = Math::HALF_PI + Math::HALF_PI * static_cast<float32>(i) / static_cast<float32>(stacks);
        float32 y   = Math::Sin(phi) * radius - halfH;
        float32 r   = -Math::Cos(phi) * radius;

        for (uint32 j = 0; j <= slices; ++j)
        {
            float32 theta = Math::TWO_PI * static_cast<float32>(j) / static_cast<float32>(slices);
            Vec3 pos(r * Math::Cos(theta), y, r * Math::Sin(theta));
            Vec3 norm = Vec3(-Math::Cos(phi) * Math::Cos(theta),
                              Math::Sin(phi),
                             -Math::Cos(phi) * Math::Sin(theta));
            float32 u = static_cast<float32>(j) / static_cast<float32>(slices);
            float32 vt = static_cast<float32>(i) / static_cast<float32>(stacks * 2);
            v.push_back(Vertex(pos, norm.Normalized(), Vec2(u, vt), w));
        }
    }

    // Индексы
    uint32 totalRows = stacks + 1 + 1 + stacks;
    for (uint32 i = 0; i < totalRows; ++i)
    {
        for (uint32 j = 0; j < slices; ++j)
        {
            uint32 a = i * (slices + 1) + j;
            uint32 b = a + 1;
            uint32 c = (i + 1) * (slices + 1) + j;
            uint32 d = c + 1;

            if (a < v.size() && b < v.size() && c < v.size() && d < v.size())
            {
                idx.push_back(a); idx.push_back(c); idx.push_back(b);
                idx.push_back(b); idx.push_back(c); idx.push_back(d);
            }
        }
    }

    auto m = MakeRef<Mesh>();
    m->Create(v, idx);
    return m;
}

} // namespace Good