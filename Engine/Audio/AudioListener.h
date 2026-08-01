#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"

namespace Good
{

    class AudioListener
    {
    public:
        static AudioListener& Get();

        void SetPosition(const Vec3& pos)    { m_Position = pos; }
        void SetForward(const Vec3& fwd)     { m_Forward  = fwd; }
        void SetUp(const Vec3& up)           { m_Up       = up;  }
        void SetVelocity(const Vec3& vel)    { m_Velocity = vel; }
        void SetVolume(float32 v)            { m_Volume   = v;   }

        const Vec3& GetPosition() const { return m_Position; }
        const Vec3& GetForward()  const { return m_Forward;  }
        const Vec3& GetUp()       const { return m_Up;       }
        const Vec3& GetVelocity() const { return m_Velocity; }
        float32     GetVolume()   const { return m_Volume;   }

    private:
        AudioListener() = default;

        Vec3    m_Position{0,0,0};
        Vec3    m_Forward{0,0,-1};
        Vec3    m_Up{0,1,0};
        Vec3    m_Velocity{0,0,0};
        float32 m_Volume{1.0f};
    };

} // namespace Good