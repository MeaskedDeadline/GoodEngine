#pragma once
#include "Core/Core.h"

namespace Good
{

    struct FontCharInfo
    {
        float32 u0{0}, v0{0}, u1{0}, v1{0};
        int32   width{0}, height{0};
        int32   xOffset{0}, yOffset{0};
        int32   advanceX{0};
    };

    class FontAtlas
    {
    public:
        static FontAtlas& Get();

        bool Initialize();
        bool LoadTTF(const std::string& path, float32 pixelHeight = 24.0f);
        void Shutdown();

        uint32 GetTextureID() const { return m_TextureID; }
        bool   IsReady()      const { return m_Ready; }
        bool   HasTTF()       const { return m_HasTTF; }

        void GetCharUV(char c, float32& u0, float32& v0,
                       float32& u1, float32& v1) const;

        const FontCharInfo& GetCharInfo(char c) const;

        float32 GetPixelHeight() const { return m_PixelHeight; }

    private:
        FontAtlas() = default;

        void BuildBuiltinAtlas();

        static constexpr int32 CHAR_W  = 8;
        static constexpr int32 CHAR_H  = 8;
        static constexpr int32 COLS    = 16;
        static constexpr int32 ROWS    = 6;
        static constexpr int32 ATLAS_W = CHAR_W * COLS;
        static constexpr int32 ATLAS_H = CHAR_H * ROWS;

        uint32       m_TextureID{0};
        bool         m_Ready{false};
        bool         m_HasTTF{false};
        float32      m_PixelHeight{16.0f};
        FontCharInfo m_Chars[128];

        static const uint8 s_BuiltinGlyphs[95][8];
    };

} // namespace Good