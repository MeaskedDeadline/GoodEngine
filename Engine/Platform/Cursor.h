#pragma once
#include "../Core/Core.h"

namespace Good
{

    enum class CursorType : uint8
    {
        Arrow = 0, Hand, IBeam, Crosshair,
        ResizeH, ResizeV, ResizeAll, NotAllowed,
        Count
    };

    class Cursor
    {
    public:
        static void SetVisible(bool visible);
        static bool IsVisible();
        static void SetLocked(bool locked);
        static bool IsLocked();
        static void SetType(CursorType type);
        static CursorType GetType();
        static void GetPosition(float32& x, float32& y);
        static void SetPosition(float32 x, float32 y);
        static const char* TypeToString(CursorType type);

    private:
        static bool       s_Visible;
        static bool       s_Locked;
        static CursorType s_Type;
    };

} // namespace Good