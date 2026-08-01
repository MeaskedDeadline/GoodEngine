#include "Gizmos/Gizmo.h"
#include "Graphics/GLFunctions.h"
#include "Math/MathCommon.h"

namespace Good
{

uint32 Gizmo::s_VAO    = 0;
uint32 Gizmo::s_VBO    = 0;
uint32 Gizmo::s_Shader = 0;
bool   Gizmo::s_Inited = false;

static const char* s_GizmoVS = R"(
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

static const char* s_GizmoFS = R"(
#version 330 core
in vec4 vColor;
out vec4 FragColor;
void main()
{
    FragColor = vColor;
}
)";

void Gizmo::InitGL()
{
    if (s_Inited) return;
    if (!GL::GenVertexArrays) return;

    GL::GenVertexArrays(1, &s_VAO);
    GL::GenBuffers(1, &s_VBO);

    GL::BindVertexArray(s_VAO);
    GL::BindBuffer(GL_ARRAY_BUFFER, s_VBO);

    GL::EnableVertexAttribArray(0);
    GL::VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28,
        reinterpret_cast<const void*>(0));
    GL::EnableVertexAttribArray(1);
    GL::VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 28,
        reinterpret_cast<const void*>(12));

    GL::BindVertexArray(0);

    if (GL::CreateShader && GL::CreateProgram)
    {
        auto compile = [](uint32 type, const char* src) -> uint32
        {
            GLuint s = GL::CreateShader(type);
            GL::ShaderSource(s, 1, &src, nullptr);
            GL::CompileShader(s);
            return s;
        };

        uint32 vs = compile(GL_VERTEX_SHADER, s_GizmoVS);
        uint32 fs = compile(GL_FRAGMENT_SHADER, s_GizmoFS);
        s_Shader = GL::CreateProgram();
        GL::AttachShader(s_Shader, vs);
        GL::AttachShader(s_Shader, fs);
        GL::LinkProgram(s_Shader);
        GL::DeleteShader(vs);
        GL::DeleteShader(fs);
    }

    s_Inited = true;
}

void Gizmo::AddLine(const Vec3& from, const Vec3& to, const Color& color)
{
    m_Lines.push_back({from, to, color});
}

void Gizmo::FlushLines(const Mat4& viewProj)
{
    if (m_Lines.empty()) return;

    InitGL();
    if (!s_VAO || !s_Shader) return;

    struct LineVert { float32 x,y,z,r,g,b,a; };

    std::vector<LineVert> verts;
    verts.reserve(m_Lines.size() * 2);

    for (auto& line : m_Lines)
    {
        verts.push_back({line.from.x, line.from.y, line.from.z,
                         line.color.r, line.color.g, line.color.b, line.color.a});
        verts.push_back({line.to.x, line.to.y, line.to.z,
                         line.color.r, line.color.g, line.color.b, line.color.a});
    }

    glDisable(GL_DEPTH_TEST);

    GL::BindVertexArray(s_VAO);
    GL::BindBuffer(GL_ARRAY_BUFFER, s_VBO);
    GL::BufferData(GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(verts.size() * sizeof(LineVert)),
        verts.data(), GL_STREAM_DRAW);

    GL::UseProgram(s_Shader);
    GLint loc = GL::GetUniformLocation(s_Shader, "u_VP");
    if (loc >= 0) GL::UniformMatrix4fv(loc, 1, GL_FALSE, viewProj.Data());

    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(verts.size()));
    glLineWidth(1.0f);

    GL::BindVertexArray(0);
    GL::UseProgram(0);

    glEnable(GL_DEPTH_TEST);

    m_Lines.clear();
}

Vec3 Gizmo::GetAxisDirection(GizmoAxis axis)
{
    switch (axis)
    {
        case GizmoAxis::X: return Vec3(1, 0, 0);
        case GizmoAxis::Y: return Vec3(0, 1, 0);
        case GizmoAxis::Z: return Vec3(0, 0, 1);
        default:           return Vec3(0, 0, 0);
    }
}

Color Gizmo::GetAxisColor(GizmoAxis axis, GizmoAxis active)
{
    Color yellow(1.0f, 0.9f, 0.2f, 1.0f);
    if (axis == active) return yellow;

    switch (axis)
    {
        case GizmoAxis::X: return Color(0.9f, 0.2f, 0.2f, 1.0f);
        case GizmoAxis::Y: return Color(0.2f, 0.9f, 0.2f, 1.0f);
        case GizmoAxis::Z: return Color(0.2f, 0.4f, 0.9f, 1.0f);
        default:           return Color(0.5f, 0.5f, 0.5f, 1.0f);
    }
}

    GizmoAxis Gizmo::HitTest(const Vec3& position, const Ray& ray, float32 threshold) const
{
    float32 len = 2.0f * m_Size;

    // Масштабируем threshold по расстоянию от камеры
    float32 dist = (position - ray.origin).Length();
    float32 scaledThreshold = threshold * dist * 0.05f;
    if (scaledThreshold < 0.2f) scaledThreshold = 0.2f;
    if (scaledThreshold > 2.0f) scaledThreshold = 2.0f;

    float32 bestDist = scaledThreshold;
    GizmoAxis bestAxis = GizmoAxis::None;

    GizmoAxis axes[] = { GizmoAxis::X, GizmoAxis::Y, GizmoAxis::Z };

    for (auto axis : axes)
    {
        Vec3 dir = GetAxisDirection(axis);
        Vec3 axisStart = position;
        Vec3 axisEnd   = position + dir * len;

        // Ближайшая точка между лучом и отрезком оси
        Vec3 u = ray.direction;
        Vec3 v = dir;
        Vec3 w = ray.origin - axisStart;

        float32 a = u.Dot(u);
        float32 b = u.Dot(v);
        float32 c = v.Dot(v);
        float32 d = u.Dot(w);
        float32 e = v.Dot(w);
        float32 denom = a * c - b * b;

        if (Math::Abs(denom) < Math::EPSILON) continue;

        float32 s = (b * e - c * d) / denom;
        float32 t = (a * e - b * d) / denom;

        // t должен быть на оси (0..len)
        if (t < 0.0f) t = 0.0f;
        if (t > len)  t = len;

        // s должен быть положительным (впереди камеры)
        if (s < 0.0f) continue;

        Vec3 closestOnRay  = ray.origin + u * s;
        Vec3 closestOnAxis = axisStart + v * t;
        float32 distance   = (closestOnRay - closestOnAxis).Length();

        if (distance < bestDist)
        {
            bestDist = distance;
            bestAxis = axis;
        }
    }

    return bestAxis;
}

} // namespace Good