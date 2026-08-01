#pragma once
#include "../Core/Core.h"
#include "../Math/Vec2.h"
#include "../Math/Color.h"
#include "../Graphics/Texture.h"

namespace Good
{

    class Sprite
    {
    public:
        Sprite() = default;

        void SetTexture(Ref<Texture> tex) { m_Texture = std::move(tex); }
        void SetColor(const Color& c)     { m_Color = c; }
        void SetSize(const Vec2& s)       { m_Size = s; }
        void SetUVOffset(const Vec2& off) { m_UVOffset = off; }
        void SetUVScale(const Vec2& scl)  { m_UVScale = scl; }

        Ref<Texture> GetTexture() const { return m_Texture; }
        const Color& GetColor()   const { return m_Color;   }
        const Vec2&  GetSize()    const { return m_Size;    }
        const Vec2&  GetUVOffset()const { return m_UVOffset;}
        const Vec2&  GetUVScale() const { return m_UVScale; }

    private:
        Ref<Texture> m_Texture;
        Color        m_Color{1,1,1,1};
        Vec2         m_Size{1,1};
        Vec2         m_UVOffset{0,0};
        Vec2         m_UVScale{1,1};
    };

} // namespace Good