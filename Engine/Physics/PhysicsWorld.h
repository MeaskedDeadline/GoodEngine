#pragma once
#include "Core/Core.h"
#include "Math/Vec3.h"
#include "Math/Quat.h"
#include "Math/Transform.h"
#include "RigidBody.h"
#include "Collider.h"
#include "PhysicsMaterial.h"
#include "Ray.h"

namespace Good
{

    struct PhysicsBody
    {
        RigidBody       rigidBody;
        Ref<Collider>   collider;
        Vec3            position{0,0,0};
        Quat            rotation;
        PhysicsMaterial material;
        bool            active{true};
    };

    class PhysicsWorld : public NonCopyable
    {
    public:
        static PhysicsWorld& Get();

        void Initialize();
        void Shutdown();
        void Step(float32 dt);

        uint32       AddBody(const PhysicsBody& body);
        void         RemoveBody(uint32 id);
        PhysicsBody* GetBody(uint32 id);

        bool                Raycast(const Ray& ray, float32 maxDist, RayHit& hit) const;
        std::vector<RayHit> RaycastAll(const Ray& ray, float32 maxDist)           const;

        void        SetGravity(const Vec3& g) { m_Gravity = g;       }
        const Vec3& GetGravity()        const { return m_Gravity;    }

    private:
        PhysicsWorld() = default;

        void ResolveCollisions();
        bool CheckCollision(const PhysicsBody& a, const PhysicsBody& b) const;

        std::unordered_map<uint32, PhysicsBody> m_Bodies;
        Vec3   m_Gravity{0.0f, -9.81f, 0.0f};
        uint32 m_NextID{1};
    };

} // namespace Good