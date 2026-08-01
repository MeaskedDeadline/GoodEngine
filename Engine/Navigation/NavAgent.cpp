#include "NavAgent.h"
#include "NavMesh.h"
#include "../Math/MathCommon.h"

namespace Good
{

    bool NavAgent::MoveTo(const Vec3& dest)
    {
        if (!m_NavMesh) return false;

        m_Destination       = dest;
        m_Reached           = false;
        m_CurrentWaypoint   = 0;

        m_HasPath = m_NavMesh->FindPath(m_Position, dest, m_Path);
        return m_HasPath;
    }

    void NavAgent::Update(float32 dt)
    {
        if (!m_HasPath || m_Reached) return;
        if (m_Path.IsEmpty())        return;

        while (m_CurrentWaypoint < m_Path.GetCount())
        {
            const Vec3& target = m_Path.GetPoint(m_CurrentWaypoint);
            Vec3 delta  = target - m_Position;
            float32 dist = delta.Length();

            if (dist < m_StopDist)
            {
                ++m_CurrentWaypoint;
                continue;
            }

            Vec3 move = delta.Normalized() * m_Speed * dt;
            if (move.Length() >= dist)
                m_Position = target;
            else
                m_Position += move;

            break;
        }

        if (m_CurrentWaypoint >= m_Path.GetCount())
        {
            m_HasPath = false;
            m_Reached = true;
        }
    }

} // namespace Good