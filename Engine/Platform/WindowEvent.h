#pragma once
#include "../Core/Core.h"

namespace Good
{

    enum class WindowEventType : uint8
    {
        None = 0,
        Close, Resize, Focus, LostFocus,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased,
        MouseMoved, MouseScrolled, CharTyped
    };

    struct WindowEvent
    {
        WindowEventType type{WindowEventType::None};
        union
        {
            struct { int32 width, height; }        resize;
            struct { int32 keyCode; bool repeat; } key;
            struct { int32 button; }               mouse;
            struct { float32 x, y; }               mousePos;
            struct { float32 xOff, yOff; }         scroll;
            struct { uint32 codepoint; }           charTyped;
        };
    };

    const char* WindowEventTypeToString(WindowEventType type);

} // namespace Good