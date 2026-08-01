#include "UI/UIRenderer.h"
#include "UI/FontAtlas.h"
#include "Graphics/GLFunctions.h"
#include "Core/Log.h"
#include "Math/MathCommon.h"
#include "Math/Mat4.h"

namespace Good
{

const char* UIRenderer::s_VertexShader = R"(
#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aUV;
layout(location=2) in vec4 aColor;
uniform mat4 u_Projection;
out vec2 vUV;
out vec4 vColor;
void main()
{
    gl_Position = u_Projection * vec4(aPos, 0.0, 1.0);
    vUV   = aUV;
    vColor= aColor;
}
)";

const char* UIRenderer::s_FragmentShader = R"(
#version 330 core
in vec2 vUV;
in vec4 vColor;
uniform sampler2D u_Texture;
out vec4 FragColor;
void main()
{
    FragColor = texture(u_Texture, vUV) * vColor;
}
)";

UIRenderer& UIRenderer::Get()
{
    static UIRenderer s_Instance;
    return s_Instance;
}

bool UIRenderer::Initialize(uint32 w, uint32 h)
{
    m_ViewportW = w;
    m_ViewportH = h;

    if (!GL::GenVertexArrays) return false;

    GL::GenVertexArrays(1, &m_VAO);
    GL::GenBuffers(1, &m_VBO);
    GL::GenBuffers(1, &m_IBO);

    GL::BindVertexArray(m_VAO);
    GL::BindBuffer(GL_ARRAY_BUFFER, m_VBO);
    GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    GL::EnableVertexAttribArray(0);
    GL::VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex),
        reinterpret_cast<const void*>(offsetof(UIVertex, position)));
    GL::EnableVertexAttribArray(1);
    GL::VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex),
        reinterpret_cast<const void*>(offsetof(UIVertex, texCoord)));
    GL::EnableVertexAttribArray(2);
    GL::VertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(UIVertex),
        reinterpret_cast<const void*>(offsetof(UIVertex, color)));

    GL::BindVertexArray(0);

    byte white[4] = {255,255,255,255};
    glGenTextures(1, &m_WhiteTexture);
    glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (GL::CreateShader && GL::CreateProgram)
    {
        auto compile = [](uint32 type, const char* src) -> uint32
        {
            GLuint s = GL::CreateShader(type);
            GL::ShaderSource(s, 1, &src, nullptr);
            GL::CompileShader(s);
            return s;
        };
        uint32 vs = compile(GL_VERTEX_SHADER, s_VertexShader);
        uint32 fs = compile(GL_FRAGMENT_SHADER, s_FragmentShader);
        m_ShaderID = GL::CreateProgram();
        GL::AttachShader(m_ShaderID, vs);
        GL::AttachShader(m_ShaderID, fs);
        GL::LinkProgram(m_ShaderID);
        GL::DeleteShader(vs);
        GL::DeleteShader(fs);
    }

    m_CurrentTexID = m_WhiteTexture;
    GOOD_LOG_INFO("UIRenderer", "Initialized ({}x{})", w, h);
    return true;
}

void UIRenderer::Shutdown()
{
    if (m_VAO && GL::DeleteVertexArrays) GL::DeleteVertexArrays(1, &m_VAO);
    if (m_VBO && GL::DeleteBuffers)      GL::DeleteBuffers(1, &m_VBO);
    if (m_IBO && GL::DeleteBuffers)      GL::DeleteBuffers(1, &m_IBO);
    if (m_WhiteTexture)                  glDeleteTextures(1, &m_WhiteTexture);
    if (m_ShaderID && GL::DeleteProgram) GL::DeleteProgram(m_ShaderID);
    m_VAO = m_VBO = m_IBO = m_WhiteTexture = m_ShaderID = 0;
}

void UIRenderer::Resize(uint32 w, uint32 h)
{
    m_ViewportW = w;
    m_ViewportH = h;
}

void UIRenderer::BeginFrame()
{
    m_Vertices.clear();
    m_Indices.clear();
    m_DrawCmds.clear();
    m_CurrentTexID   = m_WhiteTexture;
    m_LastBatchStart = 0;
}

void UIRenderer::FlushBatch()
{
    if (m_LastBatchStart < m_Indices.size())
    {
        UIDrawCmd cmd;
        cmd.indexOffset = static_cast<uint32>(m_LastBatchStart);
        cmd.indexCount  = static_cast<uint32>(m_Indices.size() - m_LastBatchStart);
        cmd.textureID   = m_CurrentTexID;
        m_DrawCmds.push_back(cmd);
        m_LastBatchStart = m_Indices.size();
    }
}

void UIRenderer::EndFrame()
{
    if (m_Vertices.empty() || !m_VAO) return;

    // Финальный batch
    FlushBatch();

    glViewport(0, 0,
        static_cast<GLsizei>(m_ViewportW),
        static_cast<GLsizei>(m_ViewportH));
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Mat4 proj = Mat4::Ortho(0.0f,
        static_cast<float32>(m_ViewportW),
        static_cast<float32>(m_ViewportH),
        0.0f, -1.0f, 1.0f);

    if (GL::UseProgram && m_ShaderID)
    {
        GL::UseProgram(m_ShaderID);
        if (GL::GetUniformLocation && GL::UniformMatrix4fv)
        {
            GLint loc = GL::GetUniformLocation(m_ShaderID, "u_Projection");
            if (loc >= 0) GL::UniformMatrix4fv(loc, 1, GL_FALSE, proj.Data());
        }
        if (GL::GetUniformLocation && GL::Uniform1i)
        {
            GLint loc = GL::GetUniformLocation(m_ShaderID, "u_Texture");
            if (loc >= 0) GL::Uniform1i(loc, 0);
        }
    }

    GL::BindVertexArray(m_VAO);

    GL::BindBuffer(GL_ARRAY_BUFFER, m_VBO);
    GL::BufferData(GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(m_Vertices.size() * sizeof(UIVertex)),
        m_Vertices.data(), GL_STREAM_DRAW);

    GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    GL::BufferData(GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(m_Indices.size() * sizeof(uint32)),
        m_Indices.data(), GL_STREAM_DRAW);

    for (auto& cmd : m_DrawCmds)
    {
        if (GL::ActiveTexture) GL::ActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cmd.textureID ? cmd.textureID : m_WhiteTexture);

        glDrawElements(GL_TRIANGLES,
            static_cast<GLsizei>(cmd.indexCount),
            GL_UNSIGNED_INT,
            reinterpret_cast<const void*>(
                static_cast<usize>(cmd.indexOffset) * sizeof(uint32)));
    }

    GL::BindVertexArray(0);
    if (GL::UseProgram) GL::UseProgram(0);
}

void UIRenderer::AddVertex(const UIVertex& v) { m_Vertices.push_back(v); }
void UIRenderer::AddIndex(uint32 idx)         { m_Indices.push_back(idx); }

void UIRenderer::AddQuad(const Vec2& tl, const Vec2& br,
                          const Vec2& uvTL, const Vec2& uvBR,
                          const Color& color, uint32 texID)
{
    // Смена текстуры — новый batch
    if (texID != m_CurrentTexID)
        FlushBatch();

    m_CurrentTexID = texID;

    uint32 base = static_cast<uint32>(m_Vertices.size());

    m_Vertices.push_back({{tl.x, tl.y}, {uvTL.x, uvTL.y}, color});
    m_Vertices.push_back({{br.x, tl.y}, {uvBR.x, uvTL.y}, color});
    m_Vertices.push_back({{br.x, br.y}, {uvBR.x, uvBR.y}, color});
    m_Vertices.push_back({{tl.x, br.y}, {uvTL.x, uvBR.y}, color});

    m_Indices.push_back(base);   m_Indices.push_back(base+1); m_Indices.push_back(base+2);
    m_Indices.push_back(base+2); m_Indices.push_back(base+3); m_Indices.push_back(base);
}

void UIRenderer::DrawRect(const Rect& r, const Color& c, float32)
{
    AddQuad({r.x, r.y}, {r.x+r.width, r.y+r.height},
            {0,0}, {1,1}, c, m_WhiteTexture);
}

void UIRenderer::DrawRectOutline(const Rect& r, const Color& c, float32 t)
{
    DrawRect(Rect(r.x,           r.y,            r.width, t),      c);
    DrawRect(Rect(r.x,           r.y+r.height-t, r.width, t),      c);
    DrawRect(Rect(r.x,           r.y,            t, r.height),     c);
    DrawRect(Rect(r.x+r.width-t, r.y,            t, r.height),     c);
}

    void UIRenderer::DrawTextUI(const std::string& text, const Vec2& pos,
                                 const Color& color, float32 fontSize)
{
    auto& font = FontAtlas::Get();
    if (!font.IsReady()) font.Initialize();

    uint32 fontTex = font.GetTextureID();
    if (fontTex == 0) return;

    float32 scale = fontSize / font.GetPixelHeight();
    if (scale < 0.1f) scale = 0.1f;

    // Baseline offset — сдвигаем текст вниз чтобы он был по центру
    float32 baselineOffset = fontSize * 0.15f;

    float32 startX = pos.x;
    float32 x      = pos.x;
    float32 y      = pos.y + baselineOffset;

    for (char ch : text)
    {
        if (ch == '\n')
        {
            x = startX;
            y += fontSize + 2.0f;
            continue;
        }

        if (ch < 32 || ch > 126)
        {
            x += fontSize * 0.5f;
            continue;
        }

        const FontCharInfo& ci = font.GetCharInfo(ch);

        if (ci.width > 0 && ci.height > 0)
        {
            float32 cx  = x + ci.xOffset * scale;
            float32 cy  = y + ci.yOffset * scale + fontSize * 0.8f;
            float32 cw  = ci.width  * scale;
            float32 ch2 = ci.height * scale;

            AddQuad(
                Vec2(cx, cy),
                Vec2(cx + cw, cy + ch2),
                Vec2(ci.u0, ci.v0),
                Vec2(ci.u1, ci.v1),
                color,
                fontTex);
        }

        x += ci.advanceX * scale;
    }
}

void UIRenderer::DrawLine(const Vec2& from, const Vec2& to,
                           const Color& color, float32 thickness)
{
    Vec2 dir = Vec2(to.x - from.x, to.y - from.y);
    float32 len = Math::Sqrt(dir.x*dir.x + dir.y*dir.y);
    if (len < 0.001f) return;

    Vec2 norm(-dir.y/len, dir.x/len);
    float32 half = thickness * 0.5f;

    uint32 base = static_cast<uint32>(m_Vertices.size());

    // Линии всегда через белую текстуру
    if (m_CurrentTexID != m_WhiteTexture) FlushBatch();
    m_CurrentTexID = m_WhiteTexture;

    m_Vertices.push_back({{from.x+norm.x*half, from.y+norm.y*half}, {0,0}, color});
    m_Vertices.push_back({{from.x-norm.x*half, from.y-norm.y*half}, {1,0}, color});
    m_Vertices.push_back({{to.x  -norm.x*half, to.y  -norm.y*half}, {1,1}, color});
    m_Vertices.push_back({{to.x  +norm.x*half, to.y  +norm.y*half}, {0,1}, color});

    m_Indices.push_back(base);   m_Indices.push_back(base+1); m_Indices.push_back(base+2);
    m_Indices.push_back(base+2); m_Indices.push_back(base+3); m_Indices.push_back(base);
}

void UIRenderer::DrawCircle(const Vec2& center, float32 radius,
                              const Color& color, int32 segs)
{
    if (m_CurrentTexID != m_WhiteTexture) FlushBatch();
    m_CurrentTexID = m_WhiteTexture;

    uint32 base = static_cast<uint32>(m_Vertices.size());
    m_Vertices.push_back({{center.x, center.y}, {0.5f,0.5f}, color});

    for (int32 i = 0; i <= segs; ++i)
    {
        float32 angle = Math::TWO_PI * static_cast<float32>(i) / static_cast<float32>(segs);
        m_Vertices.push_back({
            {center.x + Math::Cos(angle)*radius,
             center.y + Math::Sin(angle)*radius},
            {0.5f + Math::Cos(angle)*0.5f,
             0.5f + Math::Sin(angle)*0.5f},
            color});
    }

    for (int32 i = 0; i < segs; ++i)
    {
        m_Indices.push_back(base);
        m_Indices.push_back(base + 1 + i);
        m_Indices.push_back(base + 2 + i);
    }
}

void UIRenderer::DrawTriangle(const Vec2& a, const Vec2& b,
                               const Vec2& c, const Color& color)
{
    if (m_CurrentTexID != m_WhiteTexture) FlushBatch();
    m_CurrentTexID = m_WhiteTexture;

    uint32 base = static_cast<uint32>(m_Vertices.size());
    m_Vertices.push_back({{a.x,a.y}, {0,0}, color});
    m_Vertices.push_back({{b.x,b.y}, {1,0}, color});
    m_Vertices.push_back({{c.x,c.y}, {0.5f,1}, color});
    m_Indices.push_back(base); m_Indices.push_back(base+1); m_Indices.push_back(base+2);
}

void UIRenderer::DrawImage(uint32 texID, const Rect& r, const Color& tint)
{
    AddQuad({r.x, r.y}, {r.x+r.width, r.y+r.height},
            {0,0}, {1,1}, tint, texID);
}

void UIRenderer::PushClipRect(const Rect& rect) { m_ClipStack.push_back(rect); }

void UIRenderer::PopClipRect()
{
    if (!m_ClipStack.empty()) m_ClipStack.pop_back();
}

} // namespace Good