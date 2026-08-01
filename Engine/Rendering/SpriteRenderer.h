#pragma once
#include "../Core/Core.h"
#include "../Math/Mat4.h"
#include "Sprite.h"
#include "../Graphics/Shader.h"
#include "Mesh.h"

namespace Good
{

    class SpriteRenderer : public NonCopyable
    {
    public:
        static SpriteRenderer& Get();

        bool Initialize();
        void Shutdown();

        void BeginBatch(const Mat4& view, const Mat4& projection);
        void DrawSprite(const Sprite& sprite, const Mat4& transform);
        void EndBatch();

    private:
        SpriteRenderer() = default;

        Ref<Mesh>   m_QuadMesh;
        Ref<Shader> m_Shader;
        Mat4        m_VP;
        bool        m_Initialized{false};

        static const char* s_VertexShader;
        static const char* s_FragmentShader;
    };

} // namespace Good