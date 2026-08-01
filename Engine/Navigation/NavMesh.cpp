#include "NavMesh.h"
#include "../Math/MathCommon.h"

namespace Good
{

void NavMesh::AddTriangle(const NavTriangle& tri) { m_Triangles.push_back(tri); }

void NavMesh::Build()
{
    // Простая сборка — соединяем треугольники по общим рёбрам
    for (usize i = 0; i < m_Triangles.size(); ++i)
    {
        for (usize j = i+1; j < m_Triangles.size(); ++j)
        {
            int32 shared = 0;
            for (int k = 0; k < 3; ++k)
                for (int l = 0; l < 3; ++l)
                    if (m_Triangles[i].vertices[k].Distance(m_Triangles[j].vertices[l]) < 0.001f)
                        ++shared;

            if (shared >= 2)
            {
                for (int k = 0; k < 3; ++k)
                    if (m_Triangles[i].neighbors[k] == -1)
                        { m_Triangles[i].neighbors[k] = static_cast<int32>(j); break; }

                for (int k = 0; k < 3; ++k)
                    if (m_Triangles[j].neighbors[k] == -1)
                        { m_Triangles[j].neighbors[k] = static_cast<int32>(i); break; }
            }
        }
    }
    m_Built = true;
}

void NavMesh::Clear() { m_Triangles.clear(); m_Built = false; }

int32 NavMesh::FindNearestTriangle(const Vec3& point) const
{
    int32 nearest = -1;
    float32 minDist = Math::INFINITY_;

    for (int32 i = 0; i < static_cast<int32>(m_Triangles.size()); ++i)
    {
        if (!m_Triangles[i].walkable) continue;
        Vec3 center = (m_Triangles[i].vertices[0] +
                       m_Triangles[i].vertices[1] +
                       m_Triangles[i].vertices[2]) * (1.0f/3.0f);
        float32 dist = point.Distance(center);
        if (dist < minDist) { minDist = dist; nearest = i; }
    }

    return nearest;
}

bool NavMesh::FindPath(const Vec3& start, const Vec3& end, NavPath& outPath) const
{
    outPath.Clear();
    if (m_Triangles.empty()) return false;

    int32 startTri = FindNearestTriangle(start);
    int32 endTri   = FindNearestTriangle(end);

    if (startTri < 0 || endTri < 0) return false;

    // Простейший BFS по треугольникам
    std::vector<int32> parent(m_Triangles.size(), -1);
    std::vector<bool>  visited(m_Triangles.size(), false);
    std::vector<int32> queue;

    queue.push_back(startTri);
    visited[startTri] = true;

    bool found = false;
    while (!queue.empty() && !found)
    {
        int32 curr = queue.front();
        queue.erase(queue.begin());

        if (curr == endTri) { found = true; break; }

        for (int32 neighbor : m_Triangles[curr].neighbors)
        {
            if (neighbor >= 0 && !visited[neighbor] && m_Triangles[neighbor].walkable)
            {
                visited[neighbor] = true;
                parent[neighbor]  = curr;
                queue.push_back(neighbor);
            }
        }
    }

    if (!found) return false;

    std::vector<int32> path;
    int32 curr = endTri;
    while (curr != -1) { path.push_back(curr); curr = parent[curr]; }
    std::reverse(path.begin(), path.end());

    outPath.AddPoint(start);
    for (int32 tri : path)
    {
        Vec3 center = (m_Triangles[tri].vertices[0] +
                       m_Triangles[tri].vertices[1] +
                       m_Triangles[tri].vertices[2]) * (1.0f/3.0f);
        outPath.AddPoint(center);
    }
    outPath.AddPoint(end);

    return true;
}

bool NavMesh::IsWalkable(const Vec3& point) const
{
    int32 tri = FindNearestTriangle(point);
    return tri >= 0 && m_Triangles[tri].walkable;
}

} // namespace Good