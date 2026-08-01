#include "Debug/DebugDraw.h"
#include "Graphics/GLFunctions.h"
#include "Math/MathCommon.h"

namespace Good
{

std::vector<DebugDraw::DebugLine> DebugDraw::s_Lines;

uint32 DebugDraw::s_LineVAO    = 0;
uint32 DebugDraw::s_LineVBO    = 0;
uint32 DebugDraw::s_LineShader = 0;
bool   DebugDraw::s_GLInited   = false;

static const char* s_LineVS = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
uniform mat4 u_VP;
out vec4 vColor;
void main()
{
    gl_Position = u_VP * vec4(aPos, 1.0);
    vColor = aColor;
}
)";

static const char* s_LineFS = R"(
#version 330 core
in vec4 vColor;
out vec4 FragColor;
void main()
{
    FragColor = vColor;
}
)";

void DebugDraw::InitGL()
{
    if (s_GLInited) return;
    if (!GL::GenVertexArrays) return;

    GL::GenVertexArrays(1, &s_LineVAO);
    GL::GenBuffers(1, &s_LineVBO);

    GL::BindVertexArray(s_LineVAO);
    GL::BindBuffer(GL_ARRAY_BUFFER, s_LineVBO);

    // Каждый vertex: vec3 pos + vec4 color = 7 floats = 28 bytes
    GL::EnableVertexAttribArray(0);
    GL::VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, reinterpret_cast<const void*>(0));
    GL::EnableVertexAttribArray(1);
    GL::VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 28, reinterpret_cast<const void*>(12));

    GL::BindVertexArray(0);

    // Shader
    if (GL::CreateShader && GL::CreateProgram)
    {
        auto compile = [](uint32 type, const char* src) -> uint32
        {
            GLuint s = GL::CreateShader(type);
            GL::ShaderSource(s, 1, &src, nullptr);
            GL::CompileShader(s);
            return s;
        };

        uint32 vs = compile(GL_VERTEX_SHADER, s_LineVS);
        uint32 fs = compile(GL_FRAGMENT_SHADER, s_LineFS);
        s_LineShader = GL::CreateProgram();
        GL::AttachShader(s_LineShader, vs);
        GL::AttachShader(s_LineShader, fs);
        GL::LinkProgram(s_LineShader);
        GL::DeleteShader(vs);
        GL::DeleteShader(fs);
    }

    s_GLInited = true;
}

void DebugDraw::Line(const Vec3& from, const Vec3& to, const Color& color, float32 duration)
{
    s_Lines.push_back({from, to, color, duration});
}

void DebugDraw::Ray(const Vec3& origin, const Vec3& direction, float32 length,
                    const Color& color, float32 duration)
{
    Line(origin, origin + direction.Normalized() * length, color, duration);
}

void DebugDraw::Sphere(const Vec3& center, float32 radius, const Color& color, float32 duration)
{
    int32 segs = 16;
    for (int32 i = 0; i < segs; ++i)
    {
        float32 a0 = Math::TWO_PI * static_cast<float32>(i)   / segs;
        float32 a1 = Math::TWO_PI * static_cast<float32>(i+1) / segs;

        Line(center + Vec3(Math::Cos(a0)*radius, Math::Sin(a0)*radius, 0),
             center + Vec3(Math::Cos(a1)*radius, Math::Sin(a1)*radius, 0), color, duration);
        Line(center + Vec3(Math::Cos(a0)*radius, 0, Math::Sin(a0)*radius),
             center + Vec3(Math::Cos(a1)*radius, 0, Math::Sin(a1)*radius), color, duration);
        Line(center + Vec3(0, Math::Cos(a0)*radius, Math::Sin(a0)*radius),
             center + Vec3(0, Math::Cos(a1)*radius, Math::Sin(a1)*radius), color, duration);
    }
}

void DebugDraw::Box(const Vec3& center, const Vec3& size, const Color& color, float32 duration)
{
    Vec3 h = size * 0.5f;
    Vec3 c[8] = {
        center + Vec3(-h.x,-h.y,-h.z), center + Vec3( h.x,-h.y,-h.z),
        center + Vec3( h.x, h.y,-h.z), center + Vec3(-h.x, h.y,-h.z),
        center + Vec3(-h.x,-h.y, h.z), center + Vec3( h.x,-h.y, h.z),
        center + Vec3( h.x, h.y, h.z), center + Vec3(-h.x, h.y, h.z)
    };
    int32 edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}
    };
    for (auto& e : edges)
        Line(c[e[0]], c[e[1]], color, duration);
}

void DebugDraw::Axes(const Mat4& t, float32 size, float32 duration)
{
    Vec3 o = t.GetTranslation();
    Vec3 r = Vec3(t(0,0), t(0,1), t(0,2)).Normalized() * size;
    Vec3 u = Vec3(t(1,0), t(1,1), t(1,2)).Normalized() * size;
    Vec3 f = Vec3(t(2,0), t(2,1), t(2,2)).Normalized() * size;
    Line(o, o+r, Color::Red(),   duration);
    Line(o, o+u, Color::Green(), duration);
    Line(o, o+f, Color::Blue(),  duration);
}

void DebugDraw::Cross(const Vec3& pos, float32 size, const Color& color, float32 duration)
{
    Line(pos-Vec3(size,0,0), pos+Vec3(size,0,0), color, duration);
    Line(pos-Vec3(0,size,0), pos+Vec3(0,size,0), color, duration);
    Line(pos-Vec3(0,0,size), pos+Vec3(0,0,size), color, duration);
}

void DebugDraw::Grid(const Vec3& center, float32 size, int32 divs, const Color& color)
{
    float32 half = size * 0.5f;
    float32 step = size / static_cast<float32>(divs);
    for (int32 i = 0; i <= divs; ++i)
    {
        float32 t = -half + static_cast<float32>(i) * step;
        Line(center + Vec3(t, 0, -half), center + Vec3(t, 0, half), color, 0);
        Line(center + Vec3(-half, 0, t), center + Vec3(half, 0, t), color, 0);
    }
}

void DebugDraw::Update(float32 dt)
{
    s_Lines.erase(
        std::remove_if(s_Lines.begin(), s_Lines.end(),
            [dt](DebugLine& l) {
                if (l.remainingTime <= 0) return true;
                l.remainingTime -= dt;
                return false;
            }),
        s_Lines.end());
}

void DebugDraw::Flush(const Mat4& vp)
{
    if (s_Lines.empty()) return;

    InitGL();
    if (!s_LineVAO || !s_LineShader) return;

    // Собираем вершины: pos(3) + color(4) = 7 floats на vertex
    struct LineVertex { float32 x, y, z, r, g, b, a; };

    std::vector<LineVertex> verts;
    verts.reserve(s_Lines.size() * 2);

    for (auto& line : s_Lines)
    {
        verts.push_back({line.from.x, line.from.y, line.from.z,
                         line.color.r, line.color.g, line.color.b, line.color.a});
        verts.push_back({line.to.x, line.to.y, line.to.z,
                         line.color.r, line.color.g, line.color.b, line.color.a});
    }

    GL::BindVertexArray(s_LineVAO);
    GL::BindBuffer(GL_ARRAY_BUFFER, s_LineVBO);
    GL::BufferData(GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(verts.size() * sizeof(LineVertex)),
        verts.data(), GL_STREAM_DRAW);

    GL::UseProgram(s_LineShader);

    GLint vpLoc = GL::GetUniformLocation(s_LineShader, "u_VP");
    if (vpLoc >= 0) GL::UniformMatrix4fv(vpLoc, 1, GL_FALSE, vp.Data());

    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(verts.size()));
    glLineWidth(1.0f);

    GL::BindVertexArray(0);
    GL::UseProgram(0);
}

void DebugDraw::Clear()
{
    s_Lines.clear();
}

} // namespace Good