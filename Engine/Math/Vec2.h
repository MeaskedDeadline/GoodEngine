#pragma once
#include "MathCommon.h"

namespace Good
{

    struct Vec2
    {
        float32 x{0.0f};
        float32 y{0.0f};

        Vec2() = default;
        Vec2(float32 s);
        Vec2(float32 x, float32 y);

        Vec2  operator+(const Vec2& v) const;
        Vec2  operator-(const Vec2& v) const;
        Vec2  operator*(float32 s)     const;
        Vec2  operator/(float32 s)     const;
        Vec2  operator*(const Vec2& v) const;
        Vec2  operator-()              const;

        Vec2& operator+=(const Vec2& v);
        Vec2& operator-=(const Vec2& v);
        Vec2& operator*=(float32 s);
        Vec2& operator/=(float32 s);

        bool  operator==(const Vec2& v) const;
        bool  operator!=(const Vec2& v) const;

        float32& operator[](int32 i);
        float32  operator[](int32 i) const;

        float32 Length()        const;
        float32 LengthSquared() const;
        Vec2    Normalized()    const;
        void    Normalize();

        float32 Dot(const Vec2& v)   const;
        float32 Cross(const Vec2& v) const;
        float32 Distance(const Vec2& v) const;
        float32 Angle(const Vec2& v) const;

        Vec2 Lerp(const Vec2& to, float32 t) const;
        Vec2 Reflect(const Vec2& normal)     const;
        Vec2 Perpendicular()                 const;

        static Vec2 Zero();
        static Vec2 One();
        static Vec2 Up();
        static Vec2 Down();
        static Vec2 Left();
        static Vec2 Right();
    };

    Vec2 operator*(float32 s, const Vec2& v);

}