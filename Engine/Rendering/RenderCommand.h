#pragma once
#include "../Core/Core.h"
#include "../Math/Mat4.h"
#include "../Math/Color.h"

namespace Good
{

    class Mesh;
    class Shader;
    class Material;

    struct RenderCommand
    {
        Ref<Mesh>     mesh;
        Ref<Shader>   shader;
        Ref<Material> material;
        Mat4          transform;
        int32         sortKey{0};
    };

    class RenderQueue
    {
    public:
        void Push(RenderCommand cmd);
        void Sort();
        void Clear();

        const std::vector<RenderCommand>& GetCommands() const { return m_Commands; }

    private:
        std::vector<RenderCommand> m_Commands;
    };

} // namespace Good