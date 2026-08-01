#include "Math/Quat.h"
#include "Math/Vec3.h"
#include "Math/Mat4.h"

namespace Good
{

Quat::Quat(float32 x, float32 y, float32 z, float32 w)
    : x(x), y(y), z(z), w(w) {}

Quat Quat::operator*(const Quat& q) const
{
    return {
        w*q.x + x*q.w + y*q.z - z*q.y,
        w*q.y - x*q.z + y*q.w + z*q.x,
        w*q.z + x*q.y - y*q.x + z*q.w,
        w*q.w - x*q.x - y*q.y - z*q.z
    };
}

Vec3 Quat::operator*(const Vec3& v) const
{
    Vec3 u(x, y, z);
    return u * (2.0f * u.Dot(v))
         + v * (w * w - u.Dot(u))
         + u.Cross(v) * (2.0f * w);
}

Quat Quat::operator*(float32 s)      const { return {x*s, y*s, z*s, w*s}; }
Quat Quat::operator+(const Quat& q)  const { return {x+q.x, y+q.y, z+q.z, w+q.w}; }
Quat Quat::operator-()               const { return {-x, -y, -z, -w}; }

bool Quat::operator==(const Quat& q) const
{
    return Math::Approximately(x,q.x) && Math::Approximately(y,q.y)
        && Math::Approximately(z,q.z) && Math::Approximately(w,q.w);
}
bool Quat::operator!=(const Quat& q) const { return !(*this == q); }

float32 Quat::Length()        const { return Math::Sqrt(x*x+y*y+z*z+w*w); }
float32 Quat::LengthSquared() const { return x*x+y*y+z*z+w*w; }

Quat Quat::Normalized() const
{
    float32 l = Length();
    if (l < Math::EPSILON) return Identity();
    float32 inv = 1.0f / l;
    return {x*inv, y*inv, z*inv, w*inv};
}

void    Quat::Normalize()          { *this = Normalized(); }
Quat    Quat::Conjugate()    const { return {-x, -y, -z, w}; }
Quat    Quat::Inverse()      const { return Conjugate() * (1.0f / LengthSquared()); }
float32 Quat::Dot(const Quat& q)const{ return x*q.x+y*q.y+z*q.z+w*q.w; }

Vec3 Quat::ToEuler() const
{
    Vec3 e;

    float32 sp = 2.0f*(w*x+y*z);
    float32 cp = 1.0f-2.0f*(x*x+y*y);
    e.x = Math::Atan2(sp, cp) * Math::RAD2DEG;

    float32 sy = 2.0f*(w*y-z*x);
    if (Math::Abs(sy) >= 1.0f)
        e.y = (sy > 0 ? Math::HALF_PI : -Math::HALF_PI) * Math::RAD2DEG;
    else
        e.y = Math::Asin(sy) * Math::RAD2DEG;

    float32 sr = 2.0f*(w*z+x*y);
    float32 cr = 1.0f-2.0f*(y*y+z*z);
    e.z = Math::Atan2(sr, cr) * Math::RAD2DEG;

    return e;
}

Mat4 Quat::ToMatrix() const { return Mat4::FromQuat(*this); }

Vec3 Quat::Forward() const { return *this * Vec3(0,0,-1); }
Vec3 Quat::Up()      const { return *this * Vec3(0,1,0);  }
Vec3 Quat::Right()   const { return *this * Vec3(1,0,0);  }

Quat Quat::Identity() { return {0, 0, 0, 1}; }

Quat Quat::FromEuler(const Vec3& e)
{
    return FromEuler(e.x, e.y, e.z);
}

Quat Quat::FromEuler(float32 pitchDeg, float32 yawDeg, float32 rollDeg)
{
    float32 hp = Math::ToRadians(pitchDeg) * 0.5f;
    float32 hy = Math::ToRadians(yawDeg)   * 0.5f;
    float32 hr = Math::ToRadians(rollDeg)  * 0.5f;

    float32 sp = Math::Sin(hp), cp = Math::Cos(hp);
    float32 sy = Math::Sin(hy), cy = Math::Cos(hy);
    float32 sr = Math::Sin(hr), cr = Math::Cos(hr);

    return {
        cp*sy*sr + sp*cy*cr,
        cp*sy*cr - sp*cy*sr,
        cp*cy*sr - sp*sy*cr,
        cp*cy*cr + sp*sy*sr
    };
}

Quat Quat::FromAxisAngle(const Vec3& axis, float32 rad)
{
    Vec3    a = axis.Normalized();
    float32 h = rad * 0.5f;
    float32 s = Math::Sin(h);
    return {a.x*s, a.y*s, a.z*s, Math::Cos(h)};
}

Quat Quat::LookRotation(const Vec3& forward, const Vec3& up)
{
    Vec3 f = forward.Normalized();
    Vec3 r = up.Cross(f).Normalized();
    Vec3 u = f.Cross(r);

    float32 m00=r.x, m01=u.x, m02=f.x;
    float32 m10=r.y, m11=u.y, m12=f.y;
    float32 m20=r.z, m21=u.z, m22=f.z;

    float32 trace = m00+m11+m22;
    Quat q;

    if (trace > 0.0f)
    {
        float32 s = 0.5f / Math::Sqrt(trace + 1.0f);
        q.w = 0.25f / s;
        q.x = (m21-m12)*s;
        q.y = (m02-m20)*s;
        q.z = (m10-m01)*s;
    }
    else if (m00 > m11 && m00 > m22)
    {
        float32 s = 2.0f * Math::Sqrt(1.0f+m00-m11-m22);
        q.w = (m21-m12)/s;
        q.x = 0.25f*s;
        q.y = (m01+m10)/s;
        q.z = (m02+m20)/s;
    }
    else if (m11 > m22)
    {
        float32 s = 2.0f * Math::Sqrt(1.0f+m11-m00-m22);
        q.w = (m02-m20)/s;
        q.x = (m01+m10)/s;
        q.y = 0.25f*s;
        q.z = (m12+m21)/s;
    }
    else
    {
        float32 s = 2.0f * Math::Sqrt(1.0f+m22-m00-m11);
        q.w = (m10-m01)/s;
        q.x = (m02+m20)/s;
        q.y = (m12+m21)/s;
        q.z = 0.25f*s;
    }

    return q.Normalized();
}

Quat Quat::Slerp(const Quat& a, const Quat& b, float32 t)
{
    float32 dot = a.Dot(b);

    Quat b2 = b;
    if (dot < 0.0f) { b2 = -b; dot = -dot; }

    if (dot > 0.9995f)
        return Lerp(a, b2, t);

    float32 theta = Math::Acos(dot);
    float32 st    = Math::Sin(theta);

    // Используем умножение вместо деления
    float32 invSt = 1.0f / st;
    float32 wa    = Math::Sin((1.0f - t) * theta) * invSt;
    float32 wb    = Math::Sin(t * theta)           * invSt;

    return (a * wa + b2 * wb).Normalized();
}

Quat Quat::Lerp(const Quat& a, const Quat& b, float32 t)
{
    return (a * (1.0f - t) + b * t).Normalized();
}

} // namespace Good