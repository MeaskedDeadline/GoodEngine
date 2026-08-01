#include "Raycast.h"
#include "PhysicsWorld.h"

namespace Good
{

    bool Raycast::Cast(const Ray& ray, float32 maxDist, RayHit& hit)
    {
        return PhysicsWorld::Get().Raycast(ray, maxDist, hit);
    }

    std::vector<RayHit> Raycast::CastAll(const Ray& ray, float32 maxDist)
    {
        return PhysicsWorld::Get().RaycastAll(ray, maxDist);
    }

    bool Raycast::Spherecast(const Vec3& /*origin*/, float32 /*radius*/,
                              const Vec3& /*direction*/, float32 /*maxDist*/,
                              RayHit& hit)
    {
        hit.hit = false;
        return false;
    }

} // namespace Good