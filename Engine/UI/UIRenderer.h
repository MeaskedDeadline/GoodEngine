#pragma once
#include "Core/Core.h"
#include "Math/Vec2.h"
#include "Math/Color.h"
#include "Math/Mat4.h"
#include "Math/Rect.h"

namespace Good
{

struct UIVertex
{
    Vec2  position{0,0};
    Vec2  texCoord{0,0};
    Color color{1,1,1,1};
};

struct UIDrawCmd
{
    uint32 indexOffset{0};
    uint32 indexCount{0};
    uint32 textureID{0};
    Rect   clipRect{0,0,9999,9999};
};

class UIRenderer : public NonCopyable
{
public:
    static UIRenderer& Get();

    bool Initialize(uint32 viewportW, uint32 viewportH);
    void Shutdown();
    void Resize(uint32 w, uint32 h);

    void BeginFrame();
    void EndFrame();

    void DrawRect(const Rect& rect, const Color& color, float32 radius = 0.0f);
    void DrawRectOutline(const Rect& rect, const Color& color, float32 thickness = 1.0f);
    void DrawTextUI(const std::string& text, const Vec2& pos,
                    const Color& color, float32 fontSize = 14.0f);
    void DrawTriangle(const Vec2& a, const Vec2& b, const Vec2& c, const Color& color);
    void DrawLine(const Vec2& from, const Vec2& to, const Color& color, float32 thickness = 1.0f);
    void DrawCircle(const Vec2& center, float32 radius, const Color& color, int32 segments = 16);
    void DrawImage(uint32 textureID, const Rect& rect, const Color& tint = Color::White());

    void AddQuad(const Vec2& tl, const Vec2& br,
                 const Vec2& uvTL, const Vec2& uvBR,
                 const Color& color, uint32 texID);

    void PushClipRect(const Rect& rect);
    void PopClipRect();

private:
    UIRenderer() = default;

    void AddVertex(const UIVertex& v);
    void AddIndex(uint32 idx);
    void FlushBatch();

    std::vector<UIVertex>  m_Vertices;
    std::vector<uint32>    m_Indices;
    std::vector<UIDrawCmd> m_DrawCmds;
    std::vector<Rect>      m_ClipStack;

    uint32 m_VAO{0};
    uint32 m_VBO{0};
    uint32 m_IBO{0};
    uint32 m_ShaderID{0};
    uint32 m_WhiteTexture{0};
    uint32 m_ViewportW{1280};
    uint32 m_ViewportH{720};
    uint32 m_CurrentTexID{0};
    usize  m_LastBatchStart{0};

    static const char* s_VertexShader;
    static const char* s_FragmentShader;
};

} // namespace Good