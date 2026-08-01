#pragma once
#include "Vec2.h"

namespace Good
{

    struct Rect
    {
        float32 x{0},y{0},width{0},height{0};

        Rect() = default;
        Rect(float32 x, float32 y, float32 w, float32 h);

        float32 Left()   const;
        float32 Right()  const;
        float32 Top()    const;
        float32 Bottom() const;

        Vec2    Center()  const;
        Vec2    Size()    const;
        Vec2    Min()     const;
        Vec2    Max()     const;
        float32 Area()    const;

        bool Contains(const Vec2& p) const;
        bool Overlaps(const Rect& o) const;

        Rect Expanded(float32 amount) const;
        Rect Merged(const Rect& o)   const;

        static Rect FromMinMax(const Vec2& mn, const Vec2& mx);
        static Rect Zero();
    };

}