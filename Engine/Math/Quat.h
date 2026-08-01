#pragma once
#include "MathCommon.h"

namespace Good
{

    struct Vec3;
    struct Mat4;

    struct Quat
    {
        float32 x{0.0f}, y{0.0f}, z{0.0f}, w{1.0f};

        Quat() = default;
        Quat(float32 x, float32 y, float32 z, float32 w);

        Quat  operator*(const Quat& q) const;
        Vec3  operator*(const Vec3& v) const;
        Quat  operator*(float32 s)     const;
        Quat  operator+(const Quat& q) const;
        Quat  operator-()              const;

        bool  operator==(const Quat& q) const;
        bool  operator!=(const Quat& q) const;

        float32 Length()        const;
        float32 LengthSquared() const;
        Quat    Normalized()    const;
        void    Normalize();
        Quat    Conjugate()     const;
        Quat    Inverse()       const;
        float32 Dot(const Quat& q) const;

        Vec3 ToEuler()  const;
        Mat4 ToMatrix() const;

        Vec3 Forward() const;
        Vec3 Up()      const;
        Vec3 Right()   const;

        static Quat Identity();
        static Quat FromEuler(const Vec3& eulerDeg);
        static Quat FromEuler(float32 pitch, float32 yaw, float32 roll);
        static Quat FromAxisAngle(const Vec3& axis, float32 radians);
        static Quat LookRotation(const Vec3& forward, const Vec3& up);
        static Quat Slerp(const Quat& a, const Quat& b, float32 t);
        static Quat Lerp(const Quat& a, const Quat& b, float32 t);
    };

}