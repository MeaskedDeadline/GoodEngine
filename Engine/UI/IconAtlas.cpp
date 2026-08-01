#include "UI/IconAtlas.h"
#include "UI/ImageLoader.h"
#include "UI/UIRenderer.h"
#include "Graphics/GLFunctions.h"
#include "Core/Log.h"
#include <cstring>
#include <cmath>

namespace Good
{

IconAtlas& IconAtlas::Get()
{
    static IconAtlas s_Instance;
    return s_Instance;
}

bool IconAtlas::Load(const std::string& path, int32 iconW, int32 iconH)
{
    uint32 texID = ImageLoader::LoadTexture(path, true);
    if (texID == 0) return false;

    // Получаем размер текстуры
    auto image = ImageLoader::LoadFromFile(path);
    if (!image.valid) return false;

    m_TextureID  = texID;
    m_IconWidth  = iconW;
    m_IconHeight = iconH;
    m_AtlasWidth = image.width;
    m_AtlasHeight= image.height;
    m_Cols       = image.width / iconW;
    m_Rows       = image.height / iconH;
    m_Ready      = true;

    GOOD_LOG_INFO("IconAtlas", "Loaded '{}' {}x{} icons={}x{} grid={}x{}",
        path, image.width, image.height, iconW, iconH, m_Cols, m_Rows);
    return true;
}

bool IconAtlas::CreateDefault()
{
    m_IconWidth  = 32;
    m_IconHeight = 32;
    m_Cols       = 8;
    m_Rows       = 3;
    m_AtlasWidth = m_Cols * m_IconWidth;
    m_AtlasHeight= m_Rows * m_IconHeight;

    int32 aw = m_AtlasWidth;
    int32 ah = m_AtlasHeight;

    std::vector<byte> pixels(aw * ah * 4, 0);

    auto iconStart = [&](int32 index, int32& ox, int32& oy)
    {
        ox = (index % m_Cols) * m_IconWidth;
        oy = (index / m_Cols) * m_IconHeight;
    };

    int32 ox, oy;

    // 0: CURSOR — стрелка
    iconStart(CURSOR, ox, oy);
    DrawLine(pixels, aw, ox+8, oy+4, ox+8, oy+26, 220, 220, 220);
    DrawLine(pixels, aw, ox+8, oy+4, ox+16, oy+12, 220, 220, 220);
    DrawLine(pixels, aw, ox+8, oy+4, ox+4, oy+12, 220, 220, 220);
    DrawLine(pixels, aw, ox+8, oy+26, ox+12, oy+18, 220, 220, 220);
    DrawLine(pixels, aw, ox+12, oy+18, ox+16, oy+12, 220, 220, 220);

    // 1: MOVE — крестик со стрелками
    iconStart(MOVE, ox, oy);
    DrawLine(pixels, aw, ox+16, oy+4, ox+16, oy+28, 100, 220, 100);
    DrawLine(pixels, aw, ox+4, oy+16, ox+28, oy+16, 100, 220, 100);
    DrawLine(pixels, aw, ox+16, oy+4, ox+12, oy+8, 100, 220, 100);
    DrawLine(pixels, aw, ox+16, oy+4, ox+20, oy+8, 100, 220, 100);
    DrawLine(pixels, aw, ox+4, oy+16, ox+8, oy+12, 100, 220, 100);
    DrawLine(pixels, aw, ox+4, oy+16, ox+8, oy+20, 100, 220, 100);
    DrawLine(pixels, aw, ox+28, oy+16, ox+24, oy+12, 100, 220, 100);
    DrawLine(pixels, aw, ox+28, oy+16, ox+24, oy+20, 100, 220, 100);
    DrawLine(pixels, aw, ox+16, oy+28, ox+12, oy+24, 100, 220, 100);
    DrawLine(pixels, aw, ox+16, oy+28, ox+20, oy+24, 100, 220, 100);

    // 2: ROTATE — круглая стрелка
    iconStart(ROTATE, ox, oy);
    DrawCircle(pixels, aw, ox+16, oy+16, 10, 220, 180, 60);
    DrawLine(pixels, aw, ox+26, oy+14, ox+26, oy+8, 220, 180, 60);
    DrawLine(pixels, aw, ox+26, oy+14, ox+22, oy+12, 220, 180, 60);

    // 3: SCALE — квадрат с стрелкой
    iconStart(SCALE, ox, oy);
    DrawRect(pixels, aw, ox+6, oy+6, 16, 16, 180, 100, 220);
    DrawLine(pixels, aw, ox+22, oy+22, ox+28, oy+28, 180, 100, 220);
    DrawLine(pixels, aw, ox+28, oy+28, ox+24, oy+28, 180, 100, 220);
    DrawLine(pixels, aw, ox+28, oy+28, ox+28, oy+24, 180, 100, 220);

    // 4: PLAY — треугольник
    iconStart(PLAY, ox, oy);
    DrawTriangle(pixels, aw, ox+10, oy+6, ox+10, oy+26, ox+26, oy+16, 100, 220, 100);

    // 5: PAUSE — два прямоугольника
    iconStart(PAUSE, ox, oy);
    DrawFilledRect(pixels, aw, ox+8, oy+6, 5, 20, 220, 200, 60, 255);
    DrawFilledRect(pixels, aw, ox+19, oy+6, 5, 20, 220, 200, 60, 255);

    // 6: STOP — квадрат
    iconStart(STOP, ox, oy);
    DrawFilledRect(pixels, aw, ox+8, oy+8, 16, 16, 220, 80, 80, 255);

    // 7: SAVE — дискета
    iconStart(SAVE, ox, oy);
    DrawRect(pixels, aw, ox+6, oy+4, 20, 24, 180, 180, 220);
    DrawFilledRect(pixels, aw, ox+10, oy+4, 12, 10, 120, 120, 160, 255);
    DrawFilledRect(pixels, aw, ox+10, oy+18, 12, 8, 100, 100, 140, 255);

    // 8: OPEN — папка
    iconStart(OPEN, ox, oy);
    DrawFilledRect(pixels, aw, ox+4, oy+10, 24, 16, 200, 170, 60, 255);
    DrawFilledRect(pixels, aw, ox+4, oy+6, 10, 6, 200, 170, 60, 255);

    // 9: FOLDER
    iconStart(FOLDER, ox, oy);
    DrawFilledRect(pixels, aw, ox+4, oy+10, 24, 16, 220, 190, 80, 255);
    DrawFilledRect(pixels, aw, ox+4, oy+6, 10, 6, 220, 190, 80, 255);

    // 10: FILE
    iconStart(FILE, ox, oy);
    DrawRect(pixels, aw, ox+8, oy+4, 16, 24, 180, 180, 200);
    DrawLine(pixels, aw, ox+12, oy+12, ox+20, oy+12, 140, 140, 160);
    DrawLine(pixels, aw, ox+12, oy+16, ox+20, oy+16, 140, 140, 160);
    DrawLine(pixels, aw, ox+12, oy+20, ox+18, oy+20, 140, 140, 160);

    // 11: SEARCH — лупа
    iconStart(SEARCH, ox, oy);
    DrawCircle(pixels, aw, ox+14, oy+14, 8, 180, 180, 220);
    DrawLine(pixels, aw, ox+20, oy+20, ox+28, oy+28, 180, 180, 220);

    // 12: SETTINGS — шестерёнка (упрощённо)
    iconStart(SETTINGS, ox, oy);
    DrawCircle(pixels, aw, ox+16, oy+16, 10, 180, 180, 200);
    DrawCircle(pixels, aw, ox+16, oy+16, 5, 120, 120, 140);

    // 13: REFRESH — две стрелки
    iconStart(REFRESH, ox, oy);
    DrawCircle(pixels, aw, ox+16, oy+16, 9, 100, 200, 100);
    DrawLine(pixels, aw, ox+25, oy+14, ox+25, oy+8, 100, 200, 100);
    DrawLine(pixels, aw, ox+7, oy+18, ox+7, oy+24, 100, 200, 100);

    // 14: ADD — плюс
    iconStart(ADD, ox, oy);
    DrawLine(pixels, aw, ox+16, oy+6, ox+16, oy+26, 100, 220, 100);
    DrawLine(pixels, aw, ox+6, oy+16, ox+26, oy+16, 100, 220, 100);

    // 15: REMOVE — минус
    iconStart(REMOVE, ox, oy);
    DrawLine(pixels, aw, ox+6, oy+16, ox+26, oy+16, 220, 100, 100);

    // 16: EYE
    iconStart(EYE, ox, oy);
    DrawCircle(pixels, aw, ox+16, oy+16, 6, 180, 200, 220);
    DrawCircle(pixels, aw, ox+16, oy+16, 3, 100, 140, 200);

    // 17: EYE_OFF
    iconStart(EYE_OFF, ox, oy);
    DrawCircle(pixels, aw, ox+16, oy+16, 6, 120, 120, 140);
    DrawLine(pixels, aw, ox+6, oy+26, ox+26, oy+6, 220, 80, 80);

    // 18: LOCK
    iconStart(LOCK, ox, oy);
    DrawRect(pixels, aw, ox+8, oy+14, 16, 12, 220, 200, 60);
    DrawCircle(pixels, aw, ox+16, oy+12, 5, 220, 200, 60);

    // 19: UNLOCK
    iconStart(UNLOCK, ox, oy);
    DrawRect(pixels, aw, ox+8, oy+14, 16, 12, 140, 140, 160);
    DrawCircle(pixels, aw, ox+16, oy+12, 5, 140, 140, 160);

    // 20: LIGHT — солнце
    iconStart(LIGHT, ox, oy);
    DrawCircle(pixels, aw, ox+16, oy+16, 6, 255, 220, 80);
    DrawLine(pixels, aw, ox+16, oy+4, ox+16, oy+8, 255, 220, 80);
    DrawLine(pixels, aw, ox+16, oy+24, ox+16, oy+28, 255, 220, 80);
    DrawLine(pixels, aw, ox+4, oy+16, ox+8, oy+16, 255, 220, 80);
    DrawLine(pixels, aw, ox+24, oy+16, ox+28, oy+16, 255, 220, 80);

    // 21: CAMERA
    iconStart(CAMERA, ox, oy);
    DrawRect(pixels, aw, ox+6, oy+10, 20, 14, 100, 180, 220);
    DrawCircle(pixels, aw, ox+16, oy+17, 4, 100, 180, 220);
    DrawFilledRect(pixels, aw, ox+12, oy+6, 8, 4, 100, 180, 220, 255);

    // 22: CUBE
    iconStart(CUBE, ox, oy);
    DrawRect(pixels, aw, ox+8, oy+8, 16, 16, 220, 100, 100);
    DrawLine(pixels, aw, ox+8, oy+8, ox+12, oy+4, 220, 100, 100);
    DrawLine(pixels, aw, ox+24, oy+8, ox+28, oy+4, 220, 100, 100);
    DrawLine(pixels, aw, ox+12, oy+4, ox+28, oy+4, 220, 100, 100);

    // 23: SPHERE
    iconStart(SPHERE, ox, oy);
    DrawCircle(pixels, aw, ox+16, oy+16, 10, 100, 150, 220);
    DrawCircle(pixels, aw, ox+16, oy+16, 6, 80, 120, 180);

    // Создаём текстуру
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aw, ah, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    m_Ready = true;
    GOOD_LOG_INFO("IconAtlas", "Default atlas created {}x{} ({} icons)",
        aw, ah, Icon::COUNT);
    return true;
}

void IconAtlas::Shutdown()
{
    if (m_TextureID) { glDeleteTextures(1, &m_TextureID); m_TextureID = 0; }
    m_Ready = false;
}

IconInfo IconAtlas::GetIcon(int32 index) const
{
    if (!m_Ready || m_Cols == 0) return {};
    return GetIcon(index % m_Cols, index / m_Cols);
}

IconInfo IconAtlas::GetIcon(int32 col, int32 row) const
{
    if (!m_Ready) return {};

    IconInfo info;
    info.col = col;
    info.row = row;
    info.u0 = static_cast<float32>(col * m_IconWidth) / m_AtlasWidth;
    info.v0 = static_cast<float32>(row * m_IconHeight) / m_AtlasHeight;
    info.u1 = static_cast<float32>((col + 1) * m_IconWidth) / m_AtlasWidth;
    info.v1 = static_cast<float32>((row + 1) * m_IconHeight) / m_AtlasHeight;
    return info;
}

void IconAtlas::DrawIcon(int32 index, const Rect& rect, const Color& tint) const
{
    if (!m_Ready) return;
    auto icon = GetIcon(index);

    UIRenderer::Get().DrawImage(m_TextureID, rect, tint);
    // Используем AddQuad напрямую для правильных UV
    UIRenderer::Get().AddQuad(
        Vec2(rect.x, rect.y),
        Vec2(rect.x + rect.width, rect.y + rect.height),
        Vec2(icon.u0, icon.v0),
        Vec2(icon.u1, icon.v1),
        tint,
        m_TextureID);
}

void IconAtlas::DrawIcon(int32 col, int32 row, const Rect& rect, const Color& tint) const
{
    DrawIcon(row * m_Cols + col, rect, tint);
}

// ═══ Рисование примитивов в пиксельный буфер ═══

void IconAtlas::DrawPixel(std::vector<byte>& pixels, int32 aw,
                           int32 x, int32 y, byte r, byte g, byte b, byte a)
{
    if (x < 0 || y < 0 || x >= aw || y >= m_AtlasHeight) return;
    int32 idx = (y * aw + x) * 4;
    pixels[idx]   = r;
    pixels[idx+1] = g;
    pixels[idx+2] = b;
    pixels[idx+3] = a;
}

void IconAtlas::DrawLine(std::vector<byte>& pixels, int32 aw,
                          int32 x0, int32 y0, int32 x1, int32 y1,
                          byte r, byte g, byte b)
{
    int32 dx = std::abs(x1 - x0);
    int32 dy = std::abs(y1 - y0);
    int32 sx = (x0 < x1) ? 1 : -1;
    int32 sy = (y0 < y1) ? 1 : -1;
    int32 err = dx - dy;

    while (true)
    {
        DrawPixel(pixels, aw, x0, y0, r, g, b, 255);
        if (x0 == x1 && y0 == y1) break;
        int32 e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
}

void IconAtlas::DrawRect(std::vector<byte>& pixels, int32 aw,
                          int32 x, int32 y, int32 w, int32 h,
                          byte r, byte g, byte b)
{
    DrawLine(pixels, aw, x, y, x+w, y, r, g, b);
    DrawLine(pixels, aw, x+w, y, x+w, y+h, r, g, b);
    DrawLine(pixels, aw, x+w, y+h, x, y+h, r, g, b);
    DrawLine(pixels, aw, x, y+h, x, y, r, g, b);
}

void IconAtlas::DrawFilledRect(std::vector<byte>& pixels, int32 aw,
                                int32 x, int32 y, int32 w, int32 h,
                                byte r, byte g, byte b, byte a)
{
    for (int32 py = y; py < y + h; ++py)
        for (int32 px = x; px < x + w; ++px)
            DrawPixel(pixels, aw, px, py, r, g, b, a);
}

void IconAtlas::DrawCircle(std::vector<byte>& pixels, int32 aw,
                            int32 cx, int32 cy, int32 radius,
                            byte r, byte g, byte b)
{
    int32 x = radius, y = 0;
    int32 err = 1 - radius;

    while (x >= y)
    {
        DrawPixel(pixels, aw, cx+x, cy+y, r, g, b, 255);
        DrawPixel(pixels, aw, cx-x, cy+y, r, g, b, 255);
        DrawPixel(pixels, aw, cx+x, cy-y, r, g, b, 255);
        DrawPixel(pixels, aw, cx-x, cy-y, r, g, b, 255);
        DrawPixel(pixels, aw, cx+y, cy+x, r, g, b, 255);
        DrawPixel(pixels, aw, cx-y, cy+x, r, g, b, 255);
        DrawPixel(pixels, aw, cx+y, cy-x, r, g, b, 255);
        DrawPixel(pixels, aw, cx-y, cy-x, r, g, b, 255);
        y++;
        if (err < 0) err += 2*y + 1;
        else { x--; err += 2*(y-x) + 1; }
    }
}

void IconAtlas::DrawTriangle(std::vector<byte>& pixels, int32 aw,
                              int32 x0, int32 y0, int32 x1, int32 y1,
                              int32 x2, int32 y2,
                              byte r, byte g, byte b)
{
    // Заполненный треугольник через scanline
    int32 minY = std::min({y0, y1, y2});
    int32 maxY = std::max({y0, y1, y2});

    for (int32 y = minY; y <= maxY; ++y)
    {
        float32 xMin = 9999, xMax = -9999;

        auto scanEdge = [&](int32 ax, int32 ay, int32 bx, int32 by)
        {
            if ((ay <= y && by > y) || (by <= y && ay > y))
            {
                float32 t = static_cast<float32>(y - ay) / static_cast<float32>(by - ay);
                float32 ix = ax + t * (bx - ax);
                xMin = std::min(xMin, ix);
                xMax = std::max(xMax, ix);
            }
        };

        scanEdge(x0, y0, x1, y1);
        scanEdge(x1, y1, x2, y2);
        scanEdge(x2, y2, x0, y0);

        for (int32 x = static_cast<int32>(xMin); x <= static_cast<int32>(xMax); ++x)
            DrawPixel(pixels, aw, x, y, r, g, b, 255);
    }
}

} // namespace Good