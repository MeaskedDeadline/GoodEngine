#pragma once
#include "Core/Core.h"

namespace Good
{

    enum class Key : int32
    {
        Unknown = -1,

        // Буквы
        A = 65, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        // Цифры
        D0 = 48, D1, D2, D3, D4, D5, D6, D7, D8, D9,

        // F-клавиши
        F1 = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        // Стрелки
        Left     = 37,
        Up       = 38,
        Right    = 39,
        Down     = 40,

        // Навигация
        PageUp   = 33,
        PageDown = 34,
        Home     = 36,
        End      = 35,
        Insert   = 45,
        Delete   = 46,

        // Специальные
        Backspace = 8,
        Tab       = 9,
        Enter     = 13,
        Escape    = 27,
        Space     = 32,

        // Модификаторы
        LeftShift  = 160,
        RightShift = 161,
        LeftCtrl   = 162,
        RightCtrl  = 163,
        LeftAlt    = 164,
        RightAlt   = 165,
        LeftSuper  = 91,
        RightSuper = 92,

        // Numpad
        Num0 = 96, Num1, Num2, Num3, Num4,
        Num5, Num6, Num7, Num8, Num9,
        NumMultiply = 106,
        NumAdd      = 107,
        NumSubtract = 109,
        NumDecimal  = 110,
        NumDivide   = 111,

        // Пунктуация
        Semicolon    = 186,
        Equal        = 187,
        Comma        = 188,
        Minus        = 189,
        Period       = 190,
        Slash        = 191,
        GraveAccent  = 192,
        LeftBracket  = 219,
        Backslash    = 220,
        RightBracket = 221,
        Apostrophe   = 222,

        Count = 512
    };

    const char* KeyToString(Key key);

} // namespace Good