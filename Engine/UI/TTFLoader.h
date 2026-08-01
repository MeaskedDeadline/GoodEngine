#pragma once
#include "Core/Core.h"
#include <vector>

namespace Good
{

struct TTFGlyph
{
    int32   codepoint{0};
    int32   advanceWidth{0};
    int32   leftBearing{0};
    int32   xMin{0}, yMin{0}, xMax{0}, yMax{0};

    struct Contour
    {
        std::vector<float32> xPoints;
        std::vector<float32> yPoints;
        std::vector<bool>    onCurve;
    };

    std::vector<Contour> contours;
};

struct TTFAtlasChar
{
    float32 u0{0}, v0{0}, u1{0}, v1{0};
    int32   width{0}, height{0};
    int32   xOffset{0}, yOffset{0};
    int32   advanceX{0};
};

class TTFLoader
{
public:
    TTFLoader() = default;

    bool LoadFromFile(const std::string& path);
    bool LoadFromMemory(const byte* data, usize size);

    bool RasterizeAtlas(float32 pixelHeight,
                        std::vector<byte>& outPixels,
                        int32& outWidth,
                        int32& outHeight);

    const TTFAtlasChar& GetChar(char c) const;

    int32 GetAscent()  const { return m_Ascent;  }
    int32 GetDescent() const { return m_Descent; }
    int32 GetLineGap() const { return m_LineGap; }

    bool IsLoaded() const { return m_Loaded; }

private:
    // Чтение данных
    uint8  ReadU8(usize offset)  const;
    uint16 ReadU16(usize offset) const;
    int16  ReadS16(usize offset) const;
    uint32 ReadU32(usize offset) const;
    int32  ReadS32(usize offset) const;

    // Поиск таблиц
    usize FindTable(const char* tag) const;

    // Парсинг
    bool ParseHead();
    bool ParseHhea();
    bool ParseHmtx();
    bool ParseCmap();
    bool ParseLoca();
    bool ParseMaxp();

    int32 GetGlyphIndex(int32 codepoint) const;
    bool  ParseGlyph(int32 glyphIndex, TTFGlyph& outGlyph) const;
    bool  ParseSimpleGlyph(usize offset, int32 numContours, TTFGlyph& outGlyph) const;

    // Растеризация одного глифа
    void RasterizeGlyph(const TTFGlyph& glyph,
                        std::vector<byte>& bitmap,
                        int32 bitmapW, int32 bitmapH,
                        int32 offsetX, int32 offsetY,
                        float32 scale) const;

    void FillScanline(std::vector<byte>& bitmap,
                      int32 bitmapW, int32 bitmapH,
                      int32 y,
                      const std::vector<float32>& intersections,
                      int32 offsetX) const;

    std::vector<byte> m_Data;
    bool              m_Loaded{false};

    // Таблицы
    usize m_HeadOffset{0};
    usize m_HheaOffset{0};
    usize m_HmtxOffset{0};
    usize m_CmapOffset{0};
    usize m_LocaOffset{0};
    usize m_GlyfOffset{0};
    usize m_MaxpOffset{0};

    // Метрики
    int32  m_UnitsPerEm{0};
    int32  m_Ascent{0};
    int32  m_Descent{0};
    int32  m_LineGap{0};
    int32  m_NumGlyphs{0};
    int32  m_NumHMetrics{0};
    int32  m_LocaFormat{0};  // 0 = short, 1 = long

    // Cmap format 4
    std::vector<uint16> m_CmapSegStartCodes;
    std::vector<uint16> m_CmapSegEndCodes;
    std::vector<int16>  m_CmapSegIdDeltas;
    std::vector<uint16> m_CmapSegIdRangeOffsets;
    usize               m_CmapSubtableOffset{0};
    int32                m_CmapSegCount{0};

    // Advance widths
    std::vector<int32> m_AdvanceWidths;
    std::vector<int32> m_LeftBearings;

    // Atlas output
    TTFAtlasChar m_Chars[128];
};

} // namespace Good