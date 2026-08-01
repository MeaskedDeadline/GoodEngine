#pragma once
#include "Vec3.h"
#include "Quat.h"
#include "Mat4.h"

namespace Good
{

    struct Transform
    {
        Vec3 position{0,0,0};
        Quat rotation;
        Vec3 scale{1,1,1};

        Transform() = default;
        Transform(const Vec3& pos);
        Transform(const Vec3& pos, const Quat& rot);
        Transform(const Vec3& pos, const Quat& rot, const Vec3& scl);

        Mat4 ToMatrix()     const;
        Mat4 ToViewMatrix() const;

        Vec3 Forward() const;
        Vec3 Up()      const;
        Vec3 Right()   const;

        void Translate(const Vec3& delta);
        void Rotate(const Vec3& eulerDeg);
        void RotateAround(const Vec3& axis, float32 degrees);
        void LookAt(const Vec3& target, const Vec3& worldUp = Vec3::Up());

        static Transform Lerp(const Transform& a, const Transform& b, float32 t);
    };

}