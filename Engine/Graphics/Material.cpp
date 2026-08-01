#include "Material.h"
#include "../Core/Log.h"

namespace Good
{

    Material::Material() = default;

    void Material::SetShader(Ref<Shader> shader)   { m_Shader = std::move(shader); }
    void Material::SetProperties(const MaterialProperties& p) { m_Props = p; }

    void Material::SetTexture(const std::string& slot, Ref<Texture> texture)
    {
        m_Textures[slot] = std::move(texture);
    }

    void Material::Bind()
    {
        if (!m_Shader || !m_Shader->IsValid()) return;

        m_Shader->Bind();
        m_Shader->SetColor("u_Albedo",    m_Props.albedo);
        m_Shader->SetColor("u_Emissive",  m_Props.emissive);
        m_Shader->SetFloat("u_Metallic",  m_Props.metallic);
        m_Shader->SetFloat("u_Roughness", m_Props.roughness);
        m_Shader->SetFloat("u_AO",        m_Props.ao);

        m_TextureSlot = 0;
        for (auto& [name, tex] : m_Textures)
        {
            if (tex) { tex->Bind(m_TextureSlot); m_Shader->SetInt(name, static_cast<int32>(m_TextureSlot)); ++m_TextureSlot; }
        }
    }

    void Material::Unbind()
    {
        if (m_Shader) m_Shader->Unbind();
    }

    Ref<Material> Material::CreateDefault()
    {
        auto mat = MakeRef<Material>();
        mat->SetName("DefaultMaterial");
        return mat;
    }

} // namespace Good