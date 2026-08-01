#include "PhysicsWorld.h"
#include "Core/Log.h"

namespace Good
{

PhysicsWorld& PhysicsWorld::Get()
{
    static PhysicsWorld s_Instance;
    return s_Instance;
}

void PhysicsWorld::Initialize()
{
    GOOD_LOG_INFO("Physics", "PhysicsWorld initialized");
}

void PhysicsWorld::Shutdown()
{
    m_Bodies.clear();
    GOOD_LOG_INFO("Physics", "PhysicsWorld shutdown");
}

void PhysicsWorld::Step(float32 dt)
{
    for (auto& [id, body] : m_Bodies)
    {
        if (!body.active) continue;
        body.rigidBody.Integrate(body.position, body.rotation, dt);
    }

    ResolveCollisions();
}

uint32 PhysicsWorld::AddBody(const PhysicsBody& body)
{
    uint32 id = m_NextID++;
    m_Bodies[id] = body;
    return id;
}

void PhysicsWorld::RemoveBody(uint32 id)
{
    m_Bodies.erase(id);
}

PhysicsBody* PhysicsWorld::GetBody(uint32 id)
{
    auto it = m_Bodies.find(id);
    return (it != m_Bodies.end()) ? &it->second : nullptr;
}

bool PhysicsWorld::Raycast(const Ray& ray, float32 maxDist, RayHit& hit) const
{
    hit.hit = false;
    float32 closestDist = maxDist;

    for (auto& [id, body] : m_Bodies)
    {
        if (!body.active || !body.collider) continue;

        Transform tr;
        tr.position = body.position;
        tr.rotation = body.rotation;

        RayHit tmpHit;
        if (body.collider->Raycast(ray, tr, tmpHit) && tmpHit.distance < closestDist)
        {
            closestDist = tmpHit.distance;
            hit = tmpHit;
        }
    }

    return hit.hit;
}

std::vector<RayHit> PhysicsWorld::RaycastAll(const Ray& ray, float32 maxDist) const
{
    std::vector<RayHit> results;

    for (auto& [id, body] : m_Bodies)
    {
        if (!body.active || !body.collider) continue;

        Transform tr;
        tr.position = body.position;
        tr.rotation = body.rotation;

        RayHit hit;
        if (body.collider->Raycast(ray, tr, hit) && hit.distance <= maxDist)
            results.push_back(hit);
    }

    return results;
}

void PhysicsWorld::ResolveCollisions()
{
    std::vector<uint32> keys;
    keys.reserve(m_Bodies.size());
    for (auto& [id, _] : m_Bodies) keys.push_back(id);

    for (usize i = 0; i < keys.size(); ++i)
    {
        for (usize j = i + 1; j < keys.size(); ++j)
        {
            auto& a = m_Bodies[keys[i]];
            auto& b = m_Bodies[keys[j]];

            if (!a.active || !b.active)         continue;
            if (!a.collider || !b.collider)     continue;

            Transform ta, tb;
            ta.position = a.position; ta.rotation = a.rotation;
            tb.position = b.position; tb.rotation = b.rotation;

            AABB aabb_a = a.collider->GetAABB(ta);
            AABB aabb_b = b.collider->GetAABB(tb);

            if (aabb_a.Intersects(aabb_b))
            {
                Vec3 delta = b.position - a.position;
                float32 dist = delta.Length();
                if (dist < 0.001f) continue;

                Vec3 push = delta.Normalized() * 0.01f;

                if (a.rigidBody.GetBodyType() == BodyType::Dynamic)
                    a.position -= push;
                if (b.rigidBody.GetBodyType() == BodyType::Dynamic)
                    b.position += push;
            }
        }
    }
}

bool PhysicsWorld::CheckCollision(const PhysicsBody& a, const PhysicsBody& b) const
{
    if (!a.collider || !b.collider) return false;

    Transform ta, tb;
    ta.position = a.position; ta.rotation = a.rotation;
    tb.position = b.position; tb.rotation = b.rotation;

    return a.collider->GetAABB(ta).Intersects(b.collider->GetAABB(tb));
}

} // namespace Good