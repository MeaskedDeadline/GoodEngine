#pragma once
#include "../Core/Core.h"
#include "../Math/Mat4.h"
#include "../Math/Color.h"

namespace Good
{

    class Mesh;
    class Shader;
    class Material;

    struct RenderStats
    {
        uint32 drawCalls{0};
        uint32 triangles{0};
        uint32 vertices{0};
    };

    class Renderer : public NonCopyable
    {
    public:
        static Renderer& Get();

        bool Initialize();
        void Shutdown();

        void BeginScene(const Mat4& view, const Mat4& projection);
        void EndScene();

        void Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const Mat4& transform);
        void Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader,    const Mat4& transform);

        void Clear(const Color& color = Color(0.08f,0.08f,0.1f,1.0f));
        void SetViewport(int32 x, int32 y, int32 w, int32 h);
        void SetDepthTest(bool v);
        void SetBlending(bool v);
        void SetWireframe(bool v);
        void SetCullFace(bool v);

        const Mat4& GetView()       const { return m_View;       }
        const Mat4& GetProjection() const { return m_Projection; }
        const Mat4& GetVP()         const { return m_VP;         }

        const RenderStats& GetStats() const { return m_Stats; }
        void ResetStats();
        void AddDrawCall(uint32 triangles, uint32 vertices);

    private:
        Renderer() = default;

        Mat4        m_View;
        Mat4        m_Projection;
        Mat4        m_VP;
        RenderStats m_Stats;
        bool        m_Initialized{false};
    };

} // namespace Good