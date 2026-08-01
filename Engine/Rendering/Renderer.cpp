#include "Renderer.h"
#include "Mesh.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Material.h"
#include "../Graphics/GLFunctions.h"
#include "../Core/Log.h"

namespace Good
{

Renderer& Renderer::Get()
{
    static Renderer s_Instance;
    return s_Instance;
}

bool Renderer::Initialize()
{
    if (m_Initialized) return true;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    m_Initialized = true;
    GOOD_LOG_INFO("Renderer", "Initialized");
    return true;
}

void Renderer::Shutdown()
{
    m_Initialized = false;
    GOOD_LOG_INFO("Renderer", "Shutdown");
}

void Renderer::BeginScene(const Mat4& view, const Mat4& proj)
{
    ResetStats();
    m_View       = view;
    m_Projection = proj;
    m_VP         = proj * view;
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Mesh>& mesh, const Ref<Material>& mat, const Mat4& transform)
{
    if (!mesh || !mat) return;
    mat->Bind();
    if (mat->GetShader())
    {
        mat->GetShader()->SetMat4("u_VP",    m_VP);
        mat->GetShader()->SetMat4("u_Model", transform);
        Mat4 mvp = m_VP * transform;
        mat->GetShader()->SetMat4("u_MVP",   mvp);
    }
    mesh->Draw();
    AddDrawCall(mesh->GetIndexCount() / 3, mesh->GetVertexCount());
    mat->Unbind();
}

void Renderer::Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const Mat4& transform)
{
    if (!mesh || !shader) return;
    shader->Bind();
    shader->SetMat4("u_VP",    m_VP);
    shader->SetMat4("u_Model", transform);
    Mat4 mvp = m_VP * transform;
    shader->SetMat4("u_MVP",   mvp);
    mesh->Draw();
    AddDrawCall(mesh->GetIndexCount() / 3, mesh->GetVertexCount());
    shader->Unbind();
}

void Renderer::Clear(const Color& c)
{
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewport(int32 x, int32 y, int32 w, int32 h) { glViewport(x, y, w, h); }
void Renderer::SetDepthTest(bool v) { v ? glEnable(GL_DEPTH_TEST)  : glDisable(GL_DEPTH_TEST);  }
void Renderer::SetBlending(bool v)  { if (v) { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); } else glDisable(GL_BLEND); }
void Renderer::SetWireframe(bool v) { glPolygonMode(GL_FRONT_AND_BACK, v ? GL_LINE : GL_FILL); }
void Renderer::SetCullFace(bool v)  { v ? glEnable(GL_CULL_FACE)  : glDisable(GL_CULL_FACE);  }

void Renderer::ResetStats()                            { m_Stats = {}; }
void Renderer::AddDrawCall(uint32 tris, uint32 verts)  { ++m_Stats.drawCalls; m_Stats.triangles += tris; m_Stats.vertices += verts; }

} // namespace Good