#pragma once
#include "Core/Core.h"
#include "Math/Rect.h"
#include "Math/Vec2.h"
#include "Math/Color.h"

namespace Good
{

struct IconInfo
{
    float32 u0{0}, v0{0}, u1{0}, v1{0};
    int32   col{0}, row{0};
};

class IconAtlas
{
public:
    static IconAtlas& Get();

    // Загрузить атлас из файла
    bool Load(const std::string& path, int32 iconWidth = 32, int32 iconHeight = 32);

    // Создать программный атлас с простыми иконками
    bool CreateDefault();

    void Shutdown();

    // Рисовать иконку по индексу
    void DrawIcon(int32 index, const Rect& rect,
                  const Color& tint = Color::White()) const;

    // Рисовать иконку по col/row
    void DrawIcon(int32 col, int32 row, const Rect& rect,
                  const Color& tint = Color::White()) const;

    // Получить UV для иконки
    IconInfo GetIcon(int32 index) const;
    IconInfo GetIcon(int32 col, int32 row) const;

    uint32 GetTextureID()  const { return m_TextureID;  }
    int32  GetIconWidth()  const { return m_IconWidth;   }
    int32  GetIconHeight() const { return m_IconHeight;  }
    int32  GetCols()       const { return m_Cols;        }
    int32  GetRows()       const { return m_Rows;        }
    bool   IsReady()       const { return m_Ready;       }

    // Предопределённые индексы иконок
    enum Icon : int32
    {
        CURSOR = 0,
        MOVE,
        ROTATE,
        SCALE,
        PLAY,
        PAUSE,
        STOP,
        SAVE,
        OPEN,
        FOLDER,
        FILE,
        SEARCH,
        SETTINGS,
        REFRESH,
        ADD,
        REMOVE,
        EYE,
        EYE_OFF,
        LOCK,
        UNLOCK,
        LIGHT,
        CAMERA,
        CUBE,
        SPHERE,
        COUNT
    };

private:
    IconAtlas() = default;

    void DrawPixel(std::vector<byte>& pixels, int32 atlasW,
                   int32 x, int32 y, byte r, byte g, byte b, byte a);

    void DrawLine(std::vector<byte>& pixels, int32 atlasW,
                  int32 x0, int32 y0, int32 x1, int32 y1,
                  byte r, byte g, byte b);

    void DrawRect(std::vector<byte>& pixels, int32 atlasW,
                  int32 x, int32 y, int32 w, int32 h,
                  byte r, byte g, byte b);

    void DrawFilledRect(std::vector<byte>& pixels, int32 atlasW,
                        int32 x, int32 y, int32 w, int32 h,
                        byte r, byte g, byte b, byte a);

    void DrawCircle(std::vector<byte>& pixels, int32 atlasW,
                    int32 cx, int32 cy, int32 radius,
                    byte r, byte g, byte b);

    void DrawTriangle(std::vector<byte>& pixels, int32 atlasW,
                      int32 x0, int32 y0, int32 x1, int32 y1,
                      int32 x2, int32 y2,
                      byte r, byte g, byte b);

    uint32 m_TextureID{0};
    int32  m_IconWidth{32};
    int32  m_IconHeight{32};
    int32  m_AtlasWidth{0};
    int32  m_AtlasHeight{0};
    int32  m_Cols{0};
    int32  m_Rows{0};
    bool   m_Ready{false};
};

} // namespace Good