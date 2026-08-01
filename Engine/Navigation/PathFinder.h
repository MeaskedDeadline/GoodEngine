#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "NavPath.h"

namespace Good
{

    class PathFinder
    {
    public:
        struct Node
        {
            Vec3    position;
            float32 g{0};
            float32 h{0};
            float32 f() const { return g + h; }
            int32   parent{-1};
        };

        static bool FindPath(const Vec3& start, const Vec3& end,
                             const std::vector<Vec3>& waypoints,
                             NavPath& outPath);

        static float32 Heuristic(const Vec3& a, const Vec3& b);
    };

} // namespace Good