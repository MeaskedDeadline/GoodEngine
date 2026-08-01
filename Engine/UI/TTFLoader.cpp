#include "UI/TTFLoader.h"
#include "Core/Log.h"
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cmath>

namespace Good
{

// ═══ Чтение big-endian данных ═══
uint8  TTFLoader::ReadU8(usize o)  const { return m_Data[o]; }
uint16 TTFLoader::ReadU16(usize o) const { return (static_cast<uint16>(m_Data[o]) << 8) | m_Data[o+1]; }
int16  TTFLoader::ReadS16(usize o) const { return static_cast<int16>(ReadU16(o)); }
uint32 TTFLoader::ReadU32(usize o) const { return (static_cast<uint32>(m_Data[o])<<24)|(static_cast<uint32>(m_Data[o+1])<<16)|(static_cast<uint32>(m_Data[o+2])<<8)|m_Data[o+3]; }
int32  TTFLoader::ReadS32(usize o) const { return static_cast<int32>(ReadU32(o)); }

// ═══ Загрузка файла ═══
bool TTFLoader::LoadFromFile(const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        GOOD_LOG_ERROR("TTF", "Cannot open '{}'", path);
        return false;
    }

    auto size = file.tellg();
    file.seekg(0);
    m_Data.resize(static_cast<usize>(size));
    file.read(reinterpret_cast<char*>(m_Data.data()), size);

    return LoadFromMemory(m_Data.data(), m_Data.size());
}

bool TTFLoader::LoadFromMemory(const byte* data, usize size)
{
    m_Data.assign(data, data + size);

    if (size < 12)
    {
        GOOD_LOG_ERROR("TTF", "File too small");
        return false;
    }

    // Находим таблицы
    m_HeadOffset = FindTable("head");
    m_HheaOffset = FindTable("hhea");
    m_HmtxOffset = FindTable("hmtx");
    m_CmapOffset = FindTable("cmap");
    m_LocaOffset = FindTable("loca");
    m_GlyfOffset = FindTable("glyf");
    m_MaxpOffset = FindTable("maxp");

    if (!m_HeadOffset || !m_HheaOffset || !m_CmapOffset ||
        !m_LocaOffset || !m_GlyfOffset || !m_MaxpOffset)
    {
        GOOD_LOG_ERROR("TTF", "Missing required tables");
        return false;
    }

    if (!ParseMaxp()) return false;
    if (!ParseHead()) return false;
    if (!ParseHhea()) return false;
    if (!ParseHmtx()) return false;
    if (!ParseCmap()) return false;
    if (!ParseLoca()) return false;

    m_Loaded = true;
    GOOD_LOG_INFO("TTF", "Loaded: {} glyphs, {} units/em",
        m_NumGlyphs, m_UnitsPerEm);
    return true;
}

// ═══ Поиск таблицы ═══
usize TTFLoader::FindTable(const char* tag) const
{
    uint16 numTables = ReadU16(4);
    for (uint16 i = 0; i < numTables; ++i)
    {
        usize entry = 12 + i * 16;
        if (m_Data[entry]   == static_cast<byte>(tag[0]) &&
            m_Data[entry+1] == static_cast<byte>(tag[1]) &&
            m_Data[entry+2] == static_cast<byte>(tag[2]) &&
            m_Data[entry+3] == static_cast<byte>(tag[3]))
        {
            return static_cast<usize>(ReadU32(entry + 8));
        }
    }
    return 0;
}

// ═══ Парсинг таблиц ═══
bool TTFLoader::ParseMaxp()
{
    m_NumGlyphs = ReadU16(m_MaxpOffset + 4);
    return true;
}

bool TTFLoader::ParseHead()
{
    m_UnitsPerEm = ReadU16(m_HeadOffset + 18);
    m_LocaFormat = ReadS16(m_HeadOffset + 50);
    return true;
}

bool TTFLoader::ParseHhea()
{
    m_Ascent      = ReadS16(m_HheaOffset + 4);
    m_Descent     = ReadS16(m_HheaOffset + 6);
    m_LineGap     = ReadS16(m_HheaOffset + 8);
    m_NumHMetrics = ReadU16(m_HheaOffset + 34);
    return true;
}

bool TTFLoader::ParseHmtx()
{
    m_AdvanceWidths.resize(m_NumGlyphs);
    m_LeftBearings.resize(m_NumGlyphs);

    for (int32 i = 0; i < m_NumHMetrics; ++i)
    {
        usize off = m_HmtxOffset + i * 4;
        m_AdvanceWidths[i] = ReadU16(off);
        m_LeftBearings[i]  = ReadS16(off + 2);
    }

    int32 lastAdvance = m_AdvanceWidths[m_NumHMetrics - 1];
    for (int32 i = m_NumHMetrics; i < m_NumGlyphs; ++i)
    {
        m_AdvanceWidths[i] = lastAdvance;
        usize off = m_HmtxOffset + m_NumHMetrics * 4 + (i - m_NumHMetrics) * 2;
        m_LeftBearings[i] = ReadS16(off);
    }

    return true;
}

bool TTFLoader::ParseCmap()
{
    uint16 numSubtables = ReadU16(m_CmapOffset + 2);

    usize subtableOffset = 0;
    for (uint16 i = 0; i < numSubtables; ++i)
    {
        usize entry = m_CmapOffset + 4 + i * 8;
        uint16 platformID = ReadU16(entry);
        uint16 encodingID = ReadU16(entry + 2);
        uint32 offset     = ReadU32(entry + 4);

        // Windows Unicode BMP
        if (platformID == 3 && encodingID == 1)
        {
            subtableOffset = m_CmapOffset + offset;
            break;
        }
        // Unicode
        if (platformID == 0)
        {
            subtableOffset = m_CmapOffset + offset;
        }
    }

    if (!subtableOffset)
    {
        GOOD_LOG_ERROR("TTF", "No suitable cmap subtable found");
        return false;
    }

    m_CmapSubtableOffset = subtableOffset;
    uint16 format = ReadU16(subtableOffset);

    if (format == 4)
    {
        uint16 segCountX2 = ReadU16(subtableOffset + 6);
        m_CmapSegCount    = segCountX2 / 2;

        usize endCodesOff   = subtableOffset + 14;
        usize startCodesOff = endCodesOff + segCountX2 + 2;
        usize idDeltaOff    = startCodesOff + segCountX2;
        usize idRangeOff    = idDeltaOff + segCountX2;

        m_CmapSegEndCodes.resize(m_CmapSegCount);
        m_CmapSegStartCodes.resize(m_CmapSegCount);
        m_CmapSegIdDeltas.resize(m_CmapSegCount);
        m_CmapSegIdRangeOffsets.resize(m_CmapSegCount);

        for (int32 i = 0; i < m_CmapSegCount; ++i)
        {
            m_CmapSegEndCodes[i]        = ReadU16(endCodesOff + i * 2);
            m_CmapSegStartCodes[i]      = ReadU16(startCodesOff + i * 2);
            m_CmapSegIdDeltas[i]        = ReadS16(idDeltaOff + i * 2);
            m_CmapSegIdRangeOffsets[i]  = ReadU16(idRangeOff + i * 2);
        }

        return true;
    }

    GOOD_LOG_WARN("TTF", "Unsupported cmap format: {}", format);
    return false;
}

bool TTFLoader::ParseLoca()
{
    return true;
}

// ═══ Cmap lookup ═══
int32 TTFLoader::GetGlyphIndex(int32 codepoint) const
{
    if (codepoint < 0 || codepoint > 0xFFFF) return 0;

    uint16 cp = static_cast<uint16>(codepoint);

    for (int32 i = 0; i < m_CmapSegCount; ++i)
    {
        if (cp > m_CmapSegEndCodes[i]) continue;
        if (cp < m_CmapSegStartCodes[i]) return 0;

        if (m_CmapSegIdRangeOffsets[i] == 0)
        {
            return (cp + m_CmapSegIdDeltas[i]) & 0xFFFF;
        }
        else
        {
            usize idRangeOff = m_CmapSubtableOffset + 14
                             + m_CmapSegCount * 2 + 2
                             + m_CmapSegCount * 2
                             + m_CmapSegCount * 2
                             + i * 2;

            usize glyphOff = idRangeOff
                           + m_CmapSegIdRangeOffsets[i]
                           + (cp - m_CmapSegStartCodes[i]) * 2;

            if (glyphOff + 1 < m_Data.size())
            {
                uint16 idx = ReadU16(glyphOff);
                if (idx != 0)
                    return (idx + m_CmapSegIdDeltas[i]) & 0xFFFF;
            }
            return 0;
        }
    }

    return 0;
}

// ═══ Парсинг глифа ═══
bool TTFLoader::ParseGlyph(int32 glyphIndex, TTFGlyph& outGlyph) const
{
    if (glyphIndex < 0 || glyphIndex >= m_NumGlyphs) return false;

    usize glyphOffset;
    if (m_LocaFormat == 0)
        glyphOffset = m_GlyfOffset + static_cast<usize>(ReadU16(m_LocaOffset + glyphIndex * 2)) * 2;
    else
        glyphOffset = m_GlyfOffset + ReadU32(m_LocaOffset + glyphIndex * 4);

    usize nextOffset;
    if (m_LocaFormat == 0)
        nextOffset = m_GlyfOffset + static_cast<usize>(ReadU16(m_LocaOffset + (glyphIndex + 1) * 2)) * 2;
    else
        nextOffset = m_GlyfOffset + ReadU32(m_LocaOffset + (glyphIndex + 1) * 4);

    if (glyphOffset == nextOffset) return false; // empty glyph

    int16 numContours = ReadS16(glyphOffset);
    outGlyph.xMin = ReadS16(glyphOffset + 2);
    outGlyph.yMin = ReadS16(glyphOffset + 4);
    outGlyph.xMax = ReadS16(glyphOffset + 6);
    outGlyph.yMax = ReadS16(glyphOffset + 8);

    if (numContours >= 0)
        return ParseSimpleGlyph(glyphOffset, numContours, outGlyph);

    // Compound glyphs — упрощённо пропускаем
    return false;
}

bool TTFLoader::ParseSimpleGlyph(usize offset, int32 numContours, TTFGlyph& outGlyph) const
{
    if (numContours == 0) return false;

    usize pos = offset + 10;

    std::vector<uint16> endPtsOfContours(numContours);
    for (int32 i = 0; i < numContours; ++i)
    {
        endPtsOfContours[i] = ReadU16(pos);
        pos += 2;
    }

    uint16 numPoints = endPtsOfContours[numContours - 1] + 1;

    uint16 instructionLength = ReadU16(pos);
    pos += 2 + instructionLength;

    // Флаги
    std::vector<uint8> flags(numPoints);
    for (uint16 i = 0; i < numPoints;)
    {
        uint8 flag = ReadU8(pos++);
        flags[i++] = flag;

        if (flag & 0x08) // repeat
        {
            uint8 repeatCount = ReadU8(pos++);
            for (uint8 r = 0; r < repeatCount && i < numPoints; ++r)
                flags[i++] = flag;
        }
    }

    // X координаты
    std::vector<int32> xCoords(numPoints);
    int32 x = 0;
    for (uint16 i = 0; i < numPoints; ++i)
    {
        uint8 flag = flags[i];
        if (flag & 0x02) // short
        {
            uint8 dx = ReadU8(pos++);
            x += (flag & 0x10) ? dx : -static_cast<int32>(dx);
        }
        else if (!(flag & 0x10))
        {
            x += ReadS16(pos);
            pos += 2;
        }
        xCoords[i] = x;
    }

    // Y координаты
    std::vector<int32> yCoords(numPoints);
    int32 y = 0;
    for (uint16 i = 0; i < numPoints; ++i)
    {
        uint8 flag = flags[i];
        if (flag & 0x04) // short
        {
            uint8 dy = ReadU8(pos++);
            y += (flag & 0x20) ? dy : -static_cast<int32>(dy);
        }
        else if (!(flag & 0x20))
        {
            y += ReadS16(pos);
            pos += 2;
        }
        yCoords[i] = y;
    }

    // Группируем по контурам
    outGlyph.contours.resize(numContours);
    uint16 start = 0;
    for (int32 c = 0; c < numContours; ++c)
    {
        uint16 end = endPtsOfContours[c];
        auto& contour = outGlyph.contours[c];

        for (uint16 i = start; i <= end; ++i)
        {
            contour.xPoints.push_back(static_cast<float32>(xCoords[i]));
            contour.yPoints.push_back(static_cast<float32>(yCoords[i]));
            contour.onCurve.push_back((flags[i] & 0x01) != 0);
        }

        start = end + 1;
    }

    return true;
}

// ═══ Растеризация атласа ═══
bool TTFLoader::RasterizeAtlas(float32 pixelHeight,
                                std::vector<byte>& outPixels,
                                int32& outWidth,
                                int32& outHeight)
{
    if (!m_Loaded) return false;

    float32 scale = pixelHeight / static_cast<float32>(m_UnitsPerEm);

    int32 charSize = static_cast<int32>(pixelHeight) + 4;
    int32 cols = 16;
    int32 rows = 6;
    outWidth  = cols * charSize;
    outHeight = rows * charSize;

    outPixels.resize(outWidth * outHeight * 4, 0);

    for (int32 ch = 32; ch <= 126; ++ch)
    {
        int32 idx = ch - 32;
        int32 col = idx % cols;
        int32 row = idx / cols;

        int32 glyphIdx = GetGlyphIndex(ch);

        TTFGlyph glyph;
        bool hasGlyph = ParseGlyph(glyphIdx, glyph);

        TTFAtlasChar& ac = m_Chars[ch];
        ac.advanceX = static_cast<int32>(m_AdvanceWidths[glyphIdx] * scale);

        if (!hasGlyph)
        {
            ac.width = 0;
            ac.height = 0;
            continue;
        }

        int32 gw = static_cast<int32>((glyph.xMax - glyph.xMin) * scale) + 2;
        int32 gh = static_cast<int32>((glyph.yMax - glyph.yMin) * scale) + 2;

        if (gw <= 0 || gh <= 0) continue;
        if (gw > charSize) gw = charSize;
        if (gh > charSize) gh = charSize;

        int32 ox = col * charSize;
        int32 oy = row * charSize;

        ac.width   = gw;
        ac.height  = gh;
        ac.xOffset = static_cast<int32>(glyph.xMin * scale);
        ac.yOffset = static_cast<int32>(-glyph.yMax * scale);

        ac.u0 = static_cast<float32>(ox) / outWidth;
        ac.v0 = static_cast<float32>(oy) / outHeight;
        ac.u1 = static_cast<float32>(ox + gw) / outWidth;
        ac.v1 = static_cast<float32>(oy + gh) / outHeight;

        // Растеризуем глиф в bitmap
        RasterizeGlyph(glyph, outPixels, outWidth, outHeight, ox, oy, scale);
    }

    GOOD_LOG_INFO("TTF", "Atlas rasterized: {}x{} ({}px)", outWidth, outHeight, static_cast<int32>(pixelHeight));
    return true;
}

void TTFLoader::RasterizeGlyph(const TTFGlyph& glyph,
                                std::vector<byte>& bitmap,
                                int32 bmpW, int32 bmpH,
                                int32 offX, int32 offY,
                                float32 scale) const
{
    int32 gh = static_cast<int32>((glyph.yMax - glyph.yMin) * scale) + 2;

    // Суперсемплинг 4x по Y для сглаживания
    const int32 SAMPLES = 4;

    for (int32 scanY = 0; scanY < gh; ++scanY)
    {
        // Собираем coverage для каждого пикселя X
        std::unordered_map<int32, int32> coverage;

        for (int32 sub = 0; sub < SAMPLES; ++sub)
        {
            float32 subOffset = (static_cast<float32>(sub) + 0.5f) / static_cast<float32>(SAMPLES);
            float32 worldY = glyph.yMax - (static_cast<float32>(scanY) + subOffset) / scale;

            std::vector<float32> intersections;

            for (auto& contour : glyph.contours)
            {
                usize n = contour.xPoints.size();
                if (n < 2) continue;

                for (usize i = 0; i < n; ++i)
                {
                    usize j = (i + 1) % n;

                    float32 y0 = contour.yPoints[i];
                    float32 y1 = contour.yPoints[j];
                    float32 x0 = contour.xPoints[i];
                    float32 x1 = contour.xPoints[j];

                    if ((y0 <= worldY && y1 > worldY) ||
                        (y1 <= worldY && y0 > worldY))
                    {
                        float32 t = (worldY - y0) / (y1 - y0);
                        float32 ix = x0 + t * (x1 - x0);
                        intersections.push_back((ix - glyph.xMin) * scale);
                    }
                }
            }

            std::sort(intersections.begin(), intersections.end());

            // Заполняем coverage
            for (usize i = 0; i + 1 < intersections.size(); i += 2)
            {
                int32 x0 = static_cast<int32>(intersections[i]);
                int32 x1 = static_cast<int32>(std::ceil(intersections[i + 1]));

                for (int32 px = x0; px < x1; ++px)
                {
                    if (px >= 0)
                        coverage[px]++;
                }
            }
        }

        // Записываем пиксели с альфой пропорционально coverage
        int32 py = offY + scanY;
        if (py < 0 || py >= bmpH) continue;

        for (auto& [px, count] : coverage)
        {
            int32 finalX = offX + px;
            if (finalX < 0 || finalX >= bmpW) continue;

            int32 idx = (py * bmpW + finalX) * 4;

            // Alpha = coverage / SAMPLES * 255
            int32 alpha = (count * 255) / SAMPLES;
            if (alpha > 255) alpha = 255;

            // Берём максимум с существующим значением
            int32 existing = static_cast<int32>(bitmap[idx + 3]);
            if (alpha > existing)
            {
                bitmap[idx]     = 255;
                bitmap[idx + 1] = 255;
                bitmap[idx + 2] = 255;
                bitmap[idx + 3] = static_cast<byte>(alpha);
            }
        }
    }
}

void TTFLoader::FillScanline(std::vector<byte>& bitmap,
                              int32 bmpW, int32 bmpH,
                              int32 y,
                              const std::vector<float32>& intersections,
                              int32 offX) const
{
    // Этот метод больше не используется напрямую
    // Оставляем для совместимости
    if (y < 0 || y >= bmpH) return;

    for (usize i = 0; i + 1 < intersections.size(); i += 2)
    {
        int32 x0 = static_cast<int32>(intersections[i]);
        int32 x1 = static_cast<int32>(intersections[i + 1]);

        if (x0 < 0) x0 = 0;
        if (x1 > bmpW - offX) x1 = bmpW - offX;

        for (int32 x = x0; x < x1; ++x)
        {
            int32 px = offX + x;
            if (px >= 0 && px < bmpW)
            {
                int32 idx = (y * bmpW + px) * 4;
                bitmap[idx]     = 255;
                bitmap[idx + 1] = 255;
                bitmap[idx + 2] = 255;
                bitmap[idx + 3] = 255;
            }
        }
    }
}

const TTFAtlasChar& TTFLoader::GetChar(char c) const
{
    int32 idx = static_cast<int32>(c);
    if (idx >= 0 && idx < 128)
        return m_Chars[idx];

    static TTFAtlasChar s_Empty;
    return s_Empty;
}

} // namespace Good