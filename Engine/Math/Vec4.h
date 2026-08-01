#pragma once
#include "MathCommon.h"

namespace Good
{

    struct Vec3;

    struct Vec4
    {
        float32 x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f};

        Vec4() = default;
        Vec4(float32 s);
        Vec4(float32 x, float32 y, float32 z, float32 w);
        Vec4(const Vec3& v, float32 w = 1.0f);

        Vec4  operator+(const Vec4& v) const;
        Vec4  operator-(const Vec4& v) const;
        Vec4  operator*(float32 s)     const;
        Vec4  operator/(float32 s)     const;
        Vec4  operator-()              const;

        Vec4& operator+=(const Vec4& v);
        Vec4& operator-=(const Vec4& v);
        Vec4& operator*=(float32 s);
        Vec4& operator/=(float32 s);

        bool  operator==(const Vec4& v) const;
        bool  operator!=(const Vec4& v) const;

        float32& operator[](int32 i);
        float32  operator[](int32 i) const;

        float32 Length()        const;
        float32 LengthSquared() const;
        Vec4    Normalized()    const;
        void    Normalize();
        float32 Dot(const Vec4& v) const;

        Vec3 XYZ() const;

        static Vec4 Zero();
        static Vec4 One();
    };

    Vec4 operator*(float32 s, const Vec4& v);

}