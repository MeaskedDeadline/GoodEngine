#include "Cursor.h"

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

namespace Good
{

bool       Cursor::s_Visible = true;
bool       Cursor::s_Locked  = false;
CursorType Cursor::s_Type    = CursorType::Arrow;

#if defined(GOOD_PLATFORM_WINDOWS)
static POINT s_CenterScreen = {0, 0};
#endif

void Cursor::SetVisible(bool v)
{
    if (s_Visible == v) return;
    s_Visible = v;
#if defined(GOOD_PLATFORM_WINDOWS)
    if (v)
    {
        while (ShowCursor(TRUE) < 0) {}
    }
    else
    {
        while (ShowCursor(FALSE) >= 0) {}
    }
#endif
}

bool Cursor::IsVisible() { return s_Visible; }

void Cursor::SetLocked(bool locked)
{
    s_Locked = locked;
#if defined(GOOD_PLATFORM_WINDOWS)
    if (locked)
    {
        HWND hwnd = GetForegroundWindow();
        if (!hwnd) return;

        // Запоминаем центр экрана для якоря
        RECT rect;
        GetClientRect(hwnd, &rect);
        s_CenterScreen.x = (rect.right - rect.left) / 2;
        s_CenterScreen.y = (rect.bottom - rect.top) / 2;
        ClientToScreen(hwnd, &s_CenterScreen);

        // Ставим курсор в центр
        SetCursorPos(s_CenterScreen.x, s_CenterScreen.y);
    }
    else
    {
        // Ничего не делаем — SetPosition вызывается отдельно
    }
#endif
}

bool Cursor::IsLocked() { return s_Locked; }

void Cursor::SetType(CursorType type)
{
    if (s_Type == type) return;
    s_Type = type;
#if defined(GOOD_PLATFORM_WINDOWS)
    LPCSTR n = IDC_ARROW;
    switch (type)
    {
        case CursorType::Hand:       n = IDC_HAND;   break;
        case CursorType::IBeam:      n = IDC_IBEAM;  break;
        case CursorType::Crosshair:  n = IDC_CROSS;  break;
        case CursorType::ResizeH:    n = IDC_SIZEWE; break;
        case CursorType::ResizeV:    n = IDC_SIZENS; break;
        case CursorType::ResizeAll:  n = IDC_SIZEALL;break;
        case CursorType::NotAllowed: n = IDC_NO;     break;
        default: break;
    }
    ::SetCursor(LoadCursorA(nullptr, n));
#endif
}

CursorType Cursor::GetType() { return s_Type; }

void Cursor::GetPosition(float32& x, float32& y)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    POINT pt; ::GetCursorPos(&pt);
    HWND hwnd = GetForegroundWindow();
    if (hwnd) ScreenToClient(hwnd, &pt);
    x = static_cast<float32>(pt.x);
    y = static_cast<float32>(pt.y);
#else
    x = y = 0.0f;
#endif
}

void Cursor::SetPosition(float32 x, float32 y)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    POINT pt = { static_cast<LONG>(x), static_cast<LONG>(y) };
    HWND hwnd = GetForegroundWindow();
    if (hwnd) ClientToScreen(hwnd, &pt);
    ::SetCursorPos(pt.x, pt.y);
#endif
}

const char* Cursor::TypeToString(CursorType type)
{
    switch (type)
    {
        case CursorType::Arrow:      return "Arrow";
        case CursorType::Hand:       return "Hand";
        case CursorType::IBeam:      return "IBeam";
        case CursorType::Crosshair:  return "Crosshair";
        case CursorType::ResizeH:    return "ResizeH";
        case CursorType::ResizeV:    return "ResizeV";
        case CursorType::ResizeAll:  return "ResizeAll";
        case CursorType::NotAllowed: return "NotAllowed";
        default:                     return "Unknown";
    }
}

} // namespace Good