#pragma once
#include "MathCommon.h"

namespace Good
{

    struct Vec2;

    struct Vec3
    {
        float32 x{0.0f};
        float32 y{0.0f};
        float32 z{0.0f};

        Vec3() = default;
        Vec3(float32 s);
        Vec3(float32 x, float32 y, float32 z);
        Vec3(const Vec2& v, float32 z = 0.0f);

        Vec3  operator+(const Vec3& v) const;
        Vec3  operator-(const Vec3& v) const;
        Vec3  operator*(float32 s)     const;
        Vec3  operator/(float32 s)     const;
        Vec3  operator*(const Vec3& v) const;
        Vec3  operator-()              const;

        Vec3& operator+=(const Vec3& v);
        Vec3& operator-=(const Vec3& v);
        Vec3& operator*=(float32 s);
        Vec3& operator/=(float32 s);

        bool  operator==(const Vec3& v) const;
        bool  operator!=(const Vec3& v) const;

        float32& operator[](int32 i);
        float32  operator[](int32 i) const;

        float32 Length()        const;
        float32 LengthSquared() const;
        Vec3    Normalized()    const;
        void    Normalize();

        float32 Dot(const Vec3& v)   const;
        Vec3    Cross(const Vec3& v) const;
        float32 Distance(const Vec3& v) const;
        float32 Angle(const Vec3& v) const;

        Vec3 Lerp(const Vec3& to, float32 t) const;
        Vec3 Reflect(const Vec3& normal)     const;
        Vec3 Project(const Vec3& onto)       const;

        static Vec3 Zero();
        static Vec3 One();
        static Vec3 Up();
        static Vec3 Down();
        static Vec3 Left();
        static Vec3 Right();
        static Vec3 Forward();
        static Vec3 Back();
    };

    Vec3 operator*(float32 s, const Vec3& v);

}