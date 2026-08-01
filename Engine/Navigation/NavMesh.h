#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "NavPath.h"

namespace Good
{

    struct NavTriangle
    {
        Vec3   vertices[3];
        int32  neighbors[3]{-1,-1,-1};
        bool   walkable{true};
        float32 cost{1.0f};
    };

    class NavMesh
    {
    public:
        NavMesh() = default;

        void AddTriangle(const NavTriangle& tri);
        void Build();
        void Clear();

        bool FindPath(const Vec3& start, const Vec3& end, NavPath& outPath) const;
        bool IsWalkable(const Vec3& point) const;

        usize GetTriangleCount() const { return m_Triangles.size(); }

    private:
        int32 FindNearestTriangle(const Vec3& point) const;

        std::vector<NavTriangle> m_Triangles;
        bool m_Built{false};
    };

} // namespace Good