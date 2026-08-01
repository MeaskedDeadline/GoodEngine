#include "Ray.h"
#include "AABB.h"
#include "../Math/MathCommon.h"

namespace Good
{

Ray::Ray(const Vec3& o, const Vec3& d)
    : origin(o), direction(d.Normalized()) {}

Vec3 Ray::GetPoint(float32 t) const { return origin + direction * t; }

bool Ray::IntersectsAABB(const AABB& aabb, RayHit& hit) const
{
    float32 tmin = -Math::INFINITY_, tmax = Math::INFINITY_;

    for (int i = 0; i < 3; ++i)
    {
        float32 d = (&direction.x)[i];
        float32 o = (&origin.x)[i];
        float32 mn = (&aabb.min.x)[i];
        float32 mx = (&aabb.max.x)[i];

        if (Math::Abs(d) < Math::EPSILON)
        {
            if (o < mn || o > mx) { hit.hit = false; return false; }
        }
        else
        {
            float32 inv = 1.0f / d;
            float32 t1 = (mn - o) * inv;
            float32 t2 = (mx - o) * inv;
            if (t1 > t2) { float32 tmp = t1; t1 = t2; t2 = tmp; }
            tmin = Math::Max(tmin, t1);
            tmax = Math::Min(tmax, t2);
            if (tmin > tmax) { hit.hit = false; return false; }
        }
    }

    hit.hit      = (tmin >= 0.0f);
    hit.distance = tmin;
    hit.point    = GetPoint(tmin);
    return hit.hit;
}

bool Ray::IntersectsSphere(const Vec3& center, float32 radius, RayHit& hit) const
{
    Vec3 oc = origin - center;
    float32 b = oc.Dot(direction);
    float32 c = oc.Dot(oc) - radius * radius;
    float32 disc = b*b - c;

    if (disc < 0.0f) { hit.hit = false; return false; }

    float32 sqrtDisc = Math::Sqrt(disc);
    float32 t = -b - sqrtDisc;
    if (t < 0.0f) t = -b + sqrtDisc;
    if (t < 0.0f) { hit.hit = false; return false; }

    hit.hit      = true;
    hit.distance = t;
    hit.point    = GetPoint(t);
    hit.normal   = (hit.point - center).Normalized();
    return true;
}

bool Ray::IntersectsPlane(const Vec3& normal, float32 d, RayHit& hit) const
{
    float32 denom = normal.Dot(direction);
    if (Math::Abs(denom) < Math::EPSILON) { hit.hit = false; return false; }

    float32 t = -(normal.Dot(origin) + d) / denom;
    if (t < 0.0f) { hit.hit = false; return false; }

    hit.hit      = true;
    hit.distance = t;
    hit.point    = GetPoint(t);
    hit.normal   = normal;
    return true;
}

bool Ray::IntersectsTriangle(const Vec3& a, const Vec3& b, const Vec3& c, RayHit& hit) const
{
    Vec3 ab = b - a;
    Vec3 ac = c - a;
    Vec3 h  = direction.Cross(ac);
    float32 det = ab.Dot(h);

    if (Math::Abs(det) < Math::EPSILON) { hit.hit = false; return false; }

    float32 inv = 1.0f / det;
    Vec3 s  = origin - a;
    float32 u = inv * s.Dot(h);
    if (u < 0.0f || u > 1.0f) { hit.hit = false; return false; }

    Vec3 q  = s.Cross(ab);
    float32 v = inv * direction.Dot(q);
    if (v < 0.0f || u + v > 1.0f) { hit.hit = false; return false; }

    float32 t = inv * ac.Dot(q);
    if (t < 0.0f) { hit.hit = false; return false; }

    hit.hit      = true;
    hit.distance = t;
    hit.point    = GetPoint(t);
    hit.normal   = ab.Cross(ac).Normalized();
    return true;
}

} // namespace Good