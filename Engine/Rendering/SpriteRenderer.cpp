#include "SpriteRenderer.h"
#include "../Core/Log.h"
#include "Renderer.h"

namespace Good
{

    const char* SpriteRenderer::s_VertexShader = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=2) in vec2 aUV;
layout(location=3) in vec4 aColor;
uniform mat4 u_MVP;
uniform vec2 u_UVOffset;
uniform vec2 u_UVScale;
out vec2 vUV;
out vec4 vColor;
void main()
{
    gl_Position = u_MVP * vec4(aPos, 1.0);
    vUV    = aUV * u_UVScale + u_UVOffset;
    vColor = aColor;
}
)";

    const char* SpriteRenderer::s_FragmentShader = R"(
#version 330 core
in vec2 vUV;
in vec4 vColor;
uniform sampler2D u_Texture;
uniform vec4 u_Color;
out vec4 FragColor;
void main()
{
    FragColor = texture(u_Texture, vUV) * vColor * u_Color;
}
)";

    SpriteRenderer& SpriteRenderer::Get()
    {
        static SpriteRenderer s_Instance;
        return s_Instance;
    }

    bool SpriteRenderer::Initialize()
    {
        if (m_Initialized) return true;

        m_QuadMesh = Mesh::CreateQuad();
        m_Shader   = MakeRef<Shader>();
        m_Shader->LoadFromSource(s_VertexShader, s_FragmentShader);

        m_Initialized = true;
        GOOD_LOG_INFO("SpriteRenderer", "Initialized");
        return true;
    }

    void SpriteRenderer::Shutdown()
    {
        m_QuadMesh = nullptr;
        m_Shader   = nullptr;
        m_Initialized = false;
    }

    void SpriteRenderer::BeginBatch(const Mat4& view, const Mat4& proj)
    {
        m_VP = proj * view;
    }

    void SpriteRenderer::DrawSprite(const Sprite& sprite, const Mat4& transform)
    {
        if (!m_Shader || !m_QuadMesh) return;

        m_Shader->Bind();
        m_Shader->SetMat4("u_MVP",      m_VP * transform);
        m_Shader->SetColor("u_Color",   sprite.GetColor());
        m_Shader->SetVec2("u_UVOffset", sprite.GetUVOffset());
        m_Shader->SetVec2("u_UVScale",  sprite.GetUVScale());
        m_Shader->SetInt("u_Texture",   0);

        if (sprite.GetTexture()) sprite.GetTexture()->Bind(0);

        m_QuadMesh->Draw();
        Renderer::Get().AddDrawCall(2, 4);
        m_Shader->Unbind();
    }

    void SpriteRenderer::EndBatch() {}

} // namespace Good