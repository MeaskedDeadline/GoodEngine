#include "Transform.h"

namespace Good
{

    Transform::Transform(const Vec3& p) : position(p) {}
    Transform::Transform(const Vec3& p, const Quat& r) : position(p), rotation(r) {}
    Transform::Transform(const Vec3& p, const Quat& r, const Vec3& s) : position(p), rotation(r), scale(s) {}

    Mat4 Transform::ToMatrix()     const { return Mat4::TRS(position,rotation,scale); }
    Mat4 Transform::ToViewMatrix() const { return Mat4::FromQuat(rotation.Conjugate())*Mat4::Translate(-position); }

    Vec3 Transform::Forward() const { return rotation.Forward(); }
    Vec3 Transform::Up()      const { return rotation.Up(); }
    Vec3 Transform::Right()   const { return rotation.Right(); }

    void Transform::Translate(const Vec3& d) { position+=d; }

    void Transform::Rotate(const Vec3& e)
    {
        rotation=rotation*Quat::FromEuler(e);
        rotation.Normalize();
    }

    void Transform::RotateAround(const Vec3& axis, float32 deg)
    {
        rotation=Quat::FromAxisAngle(axis,Math::ToRadians(deg))*rotation;
        rotation.Normalize();
    }

    void Transform::LookAt(const Vec3& target, const Vec3& up)
    {
        Vec3 dir=(target-position).Normalized();
        if(dir.LengthSquared()<Math::EPSILON) return;
        rotation=Quat::LookRotation(dir,up);
    }

    Transform Transform::Lerp(const Transform& a, const Transform& b, float32 t)
    {
        Transform r;
        r.position=a.position.Lerp(b.position,t);
        r.rotation=Quat::Slerp(a.rotation,b.rotation,t);
        r.scale=a.scale.Lerp(b.scale,t);
        return r;
    }

}