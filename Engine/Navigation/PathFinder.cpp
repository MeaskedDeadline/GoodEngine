#include "PathFinder.h"
#include "../Math/MathCommon.h"
#include <algorithm>

namespace Good
{

    float32 PathFinder::Heuristic(const Vec3& a, const Vec3& b)
    {
        return a.Distance(b);
    }

    bool PathFinder::FindPath(const Vec3& start, const Vec3& end,
                               const std::vector<Vec3>& waypoints,
                               NavPath& outPath)
    {
        outPath.Clear();

        if (waypoints.empty())
        {
            outPath.AddPoint(start);
            outPath.AddPoint(end);
            return true;
        }

        // Простой A* по waypoints
        int32 n = static_cast<int32>(waypoints.size());
        std::vector<Node> nodes(n);
        for (int32 i = 0; i < n; ++i)
        {
            nodes[i].position = waypoints[i];
            nodes[i].g        = start.Distance(waypoints[i]);
            nodes[i].h        = waypoints[i].Distance(end);
        }

        std::sort(nodes.begin(), nodes.end(),
            [](const Node& a, const Node& b) { return a.f() < b.f(); });

        outPath.AddPoint(start);
        for (usize i = 0; i < Math::Min(static_cast<usize>(3), nodes.size()); ++i)
            outPath.AddPoint(nodes[i].position);
        outPath.AddPoint(end);

        return true;
    }

} // namespace Good