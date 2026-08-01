#pragma once
#include "Core/Core.h"

namespace Good
{

    class BuiltinFont
    {
    public:
        // 8x8 битовая карта для ASCII 32-126
        static const uint8* GetGlyphBitmap(char c);
        static constexpr int32 CHAR_WIDTH  = 8;
        static constexpr int32 CHAR_HEIGHT = 8;
    };

} // namespace Good