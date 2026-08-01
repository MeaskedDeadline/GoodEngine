#pragma once
#include "MathCommon.h"

namespace Good
{

    struct Vec3;
    struct Vec4;
    struct Quat;

    struct Mat4
    {
        float32 m[4][4];

        Mat4();
        explicit Mat4(float32 diagonal);
        Mat4(const float32* data16);

        Mat4  operator*(const Mat4& o)  const;
        Vec4  operator*(const Vec4& v)  const;
        Mat4  operator*(float32 s)      const;
        Mat4  operator+(const Mat4& o)  const;
        Mat4& operator*=(const Mat4& o);

        bool  operator==(const Mat4& o) const;
        bool  operator!=(const Mat4& o) const;

        float32& operator()(int32 col, int32 row);
        float32  operator()(int32 col, int32 row) const;

        Mat4    Transposed()     const;
        Mat4    Inverted()       const;
        float32 Determinant()    const;
        Vec3    GetTranslation() const;
        Vec3    GetScale()       const;
        const float32* Data()    const;

        static Mat4 Identity();
        static Mat4 Translate(const Vec3& t);
        static Mat4 Scale(const Vec3& s);
        static Mat4 RotateX(float32 rad);
        static Mat4 RotateY(float32 rad);
        static Mat4 RotateZ(float32 rad);
        static Mat4 Rotate(const Vec3& axis, float32 rad);
        static Mat4 FromQuat(const Quat& q);

        static Mat4 Perspective(float32 fovY, float32 aspect, float32 near, float32 far);
        static Mat4 Ortho(float32 l, float32 r, float32 b, float32 t, float32 n, float32 f);
        static Mat4 LookAt(const Vec3& eye, const Vec3& target, const Vec3& up);
        static Mat4 TRS(const Vec3& t, const Quat& r, const Vec3& s);
    };

}