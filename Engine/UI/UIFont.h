#pragma once
#include "../Core/Core.h"

namespace Good
{

    struct GlyphInfo
    {
        float32 x{0}, y{0};
        float32 width{0}, height{0};
        float32 xOffset{0}, yOffset{0};
        float32 xAdvance{0};
        uint32  texPage{0};
    };

    class UIFont
    {
    public:
        UIFont() = default;

        bool LoadBitmapFont(const std::string& fntPath);

        float32 GetFontSize()    const { return m_FontSize;    }
        float32 GetLineHeight()  const { return m_LineHeight;  }
        uint32  GetTexture(uint32 page = 0) const;

        bool    HasGlyph(uint32 codepoint) const;
        const GlyphInfo& GetGlyph(uint32 codepoint) const;

        float32 MeasureText(const std::string& text, float32 fontSize = -1.0f) const;
        float32 GetKerning(uint32 first, uint32 second) const;

    private:
        float32 m_FontSize{14.0f};
        float32 m_LineHeight{18.0f};

        std::unordered_map<uint32, GlyphInfo>          m_Glyphs;
        std::unordered_map<uint64, float32>            m_Kerning;
        std::vector<uint32>                            m_Textures;

        static GlyphInfo s_DefaultGlyph;
    };

} // namespace Good