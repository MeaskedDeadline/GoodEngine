#include "UIFont.h"
#include "../Core/Log.h"
#include <fstream>
#include <sstream>

namespace Good
{

GlyphInfo UIFont::s_DefaultGlyph = {};

bool UIFont::LoadBitmapFont(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        GOOD_LOG_ERROR("UIFont", "Cannot open font: '{}'", path);
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string tag;
        iss >> tag;

        auto getVal = [&](const std::string& key) -> int32
        {
            usize pos = line.find(key + "=");
            if (pos == std::string::npos) return 0;
            pos += key.size() + 1;
            return std::stoi(line.substr(pos));
        };

        if (tag == "info")
        {
            m_FontSize = static_cast<float32>(std::abs(getVal("size")));
        }
        else if (tag == "common")
        {
            m_LineHeight = static_cast<float32>(getVal("lineHeight"));
        }
        else if (tag == "char")
        {
            uint32 id = static_cast<uint32>(getVal("id"));
            GlyphInfo g;
            g.x        = static_cast<float32>(getVal("x"));
            g.y        = static_cast<float32>(getVal("y"));
            g.width    = static_cast<float32>(getVal("width"));
            g.height   = static_cast<float32>(getVal("height"));
            g.xOffset  = static_cast<float32>(getVal("xoffset"));
            g.yOffset  = static_cast<float32>(getVal("yoffset"));
            g.xAdvance = static_cast<float32>(getVal("xadvance"));
            g.texPage  = static_cast<uint32>(getVal("page"));
            m_Glyphs[id] = g;
        }
        else if (tag == "kerning")
        {
            uint32 first  = static_cast<uint32>(getVal("first"));
            uint32 second = static_cast<uint32>(getVal("second"));
            float32 amt   = static_cast<float32>(getVal("amount"));
            uint64 key    = (static_cast<uint64>(first) << 32) | second;
            m_Kerning[key] = amt;
        }
    }

    GOOD_LOG_INFO("UIFont", "Loaded font '{}' ({}px, {} glyphs)",
        path, static_cast<int32>(m_FontSize), m_Glyphs.size());
    return true;
}

bool UIFont::HasGlyph(uint32 cp) const
{
    return m_Glyphs.find(cp) != m_Glyphs.end();
}

const GlyphInfo& UIFont::GetGlyph(uint32 cp) const
{
    auto it = m_Glyphs.find(cp);
    return (it != m_Glyphs.end()) ? it->second : s_DefaultGlyph;
}

float32 UIFont::GetKerning(uint32 first, uint32 second) const
{
    uint64 key = (static_cast<uint64>(first) << 32) | second;
    auto it    = m_Kerning.find(key);
    return (it != m_Kerning.end()) ? it->second : 0.0f;
}

float32 UIFont::MeasureText(const std::string& text, float32 fontSize) const
{
    float32 scale = (fontSize > 0.0f) ? fontSize / m_FontSize : 1.0f;
    float32 width = 0.0f;
    for (usize i = 0; i < text.size(); ++i)
    {
        uint32 cp = static_cast<uint32>(text[i]);
        if (HasGlyph(cp)) width += GetGlyph(cp).xAdvance * scale;
        if (i + 1 < text.size()) width += GetKerning(cp, static_cast<uint32>(text[i+1])) * scale;
    }
    return width;
}

uint32 UIFont::GetTexture(uint32 page) const
{
    return page < m_Textures.size() ? m_Textures[page] : 0;
}

} // namespace Good