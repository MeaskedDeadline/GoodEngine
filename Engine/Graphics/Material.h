#pragma once
#include "../Core/Core.h"
#include "../Math/Color.h"
#include "../Math/Vec3.h"
#include "Shader.h"
#include "Texture.h"

namespace Good
{

    struct MaterialProperties
    {
        Color   albedo{1,1,1,1};
        Color   emissive{0,0,0,1};
        float32 metallic{0.0f};
        float32 roughness{0.5f};
        float32 ao{1.0f};
        bool    isTransparent{false};
        bool    castShadows{true};
        bool    receiveShadows{true};
    };

    class Material
    {
    public:
        Material();
        ~Material() = default;

        void SetShader(Ref<Shader> shader);
        void SetTexture(const std::string& slot, Ref<Texture> texture);
        void SetProperties(const MaterialProperties& props);

        Ref<Shader>  GetShader()  const { return m_Shader;  }
        const MaterialProperties& GetProperties() const { return m_Props; }

        void Bind();
        void Unbind();

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        static Ref<Material> CreateDefault();

    private:
        std::string                             m_Name{"Material"};
        Ref<Shader>                             m_Shader;
        std::unordered_map<std::string, Ref<Texture>> m_Textures;
        MaterialProperties                      m_Props;
        uint32                                  m_TextureSlot{0};
    };

} // namespace Good