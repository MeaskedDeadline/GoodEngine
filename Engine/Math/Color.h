#pragma once
#include "MathCommon.h"

namespace Good
{

    struct Color
    {
        float32 r{1},g{1},b{1},a{1};

        Color() = default;
        Color(float32 r, float32 g, float32 b, float32 a = 1.0f);

        Color operator*(float32 s) const;
        Color operator+(const Color& c) const;
        Color operator*(const Color& c) const;

        bool operator==(const Color& c) const;
        bool operator!=(const Color& c) const;

        uint32 ToRGBA8()  const;
        uint32 ToABGR8()  const;
        Color  Lerp(const Color& to, float32 t) const;

        static Color FromRGBA8(uint32 rgba);
        static Color FromHSV(float32 h, float32 s, float32 v, float32 a = 1.0f);

        static Color White();
        static Color Black();
        static Color Red();
        static Color Green();
        static Color Blue();
        static Color Yellow();
        static Color Cyan();
        static Color Magenta();
        static Color Gray();
        static Color Clear();
    };

}