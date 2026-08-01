#pragma once
#include "Core/Core.h"
#include "Graphics/Texture.h"

namespace Good
{

    class EditorFont
    {
    public:
        static EditorFont& Get();

        bool Initialize();
        void Shutdown();

        void BindTexture(uint32 slot = 0) const;

        uint32 GetTextureID()  const { return m_TextureID; }
        int32  GetCharWidth()  const { return CHAR_W; }
        int32  GetCharHeight() const { return CHAR_H; }
        int32  GetAtlasWidth() const { return ATLAS_W; }
        int32  GetAtlasHeight()const { return ATLAS_H; }

        // UV координаты символа
        void GetCharUV(char c, float32& u0, float32& v0,
                       float32& u1, float32& v1) const;

        bool IsInitialized() const { return m_Initialized; }

    private:
        EditorFont() = default;

        void GenerateAtlas();

        static constexpr int32 CHAR_W  = 8;
        static constexpr int32 CHAR_H  = 8;
        static constexpr int32 COLS    = 16;
        static constexpr int32 ROWS    = 6;
        static constexpr int32 ATLAS_W = CHAR_W * COLS;  // 128
        static constexpr int32 ATLAS_H = CHAR_H * ROWS;  // 48

        uint32 m_TextureID{0};
        bool   m_Initialized{false};

        static const uint8 s_FontData[95][8];
    };

} // namespace Good