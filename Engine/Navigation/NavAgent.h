#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "NavPath.h"

namespace Good
{

    class NavMesh;

    class NavAgent
    {
    public:
        NavAgent() = default;

        void SetNavMesh(NavMesh* navMesh) { m_NavMesh = navMesh; }
        void SetPosition(const Vec3& pos) { m_Position = pos; }
        void SetSpeed(float32 s)          { m_Speed    = s;   }
        void SetStopDistance(float32 d)   { m_StopDist = d;   }

        bool MoveTo(const Vec3& destination);
        void Stop()  { m_HasPath = false; m_CurrentWaypoint = 0; }
        void Update(float32 dt);

        const Vec3& GetPosition()    const { return m_Position;    }
        const Vec3& GetDestination() const { return m_Destination; }
        bool        HasPath()        const { return m_HasPath;     }
        bool        HasReached()     const { return m_Reached;     }
        float32     GetSpeed()       const { return m_Speed;       }

    private:
        NavMesh*  m_NavMesh{nullptr};
        NavPath   m_Path;
        Vec3      m_Position{0,0,0};
        Vec3      m_Destination{0,0,0};
        usize     m_CurrentWaypoint{0};
        float32   m_Speed{3.0f};
        float32   m_StopDist{0.1f};
        bool      m_HasPath{false};
        bool      m_Reached{false};
    };

} // namespace Good