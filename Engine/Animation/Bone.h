#pragma once
#include "Core/Core.h"
#include "Math/Transform.h"
#include "Math/Mat4.h"

namespace Good
{

    class Bone
    {
    public:
        Bone() = default;
        Bone(const std::string& name, int32 parentIndex = -1);

        const std::string& GetName()        const { return m_Name;        }
        int32              GetParentIndex() const { return m_ParentIndex; }

        void SetLocalTransform(const Transform& t) { m_LocalTransform = t; }
        void SetBindPose(const Mat4& m)            { m_BindPose = m;       }
        void SetInverseBindPose(const Mat4& m)     { m_InverseBindPose = m;}

        const Transform& GetLocalTransform()   const { return m_LocalTransform;  }
        const Mat4&      GetBindPose()         const { return m_BindPose;        }
        const Mat4&      GetInverseBindPose()  const { return m_InverseBindPose; }

    private:
        std::string m_Name{"Bone"};
        int32       m_ParentIndex{-1};
        Transform   m_LocalTransform;
        Mat4        m_BindPose;
        Mat4        m_InverseBindPose;
    };

} // namespace Good