#include "Skeleton.h"

namespace Good
{

    void Skeleton::AddBone(const Bone& bone) { m_Bones.push_back(bone); }

    int32 Skeleton::FindBone(const std::string& name) const
    {
        for (int32 i = 0; i < static_cast<int32>(m_Bones.size()); ++i)
            if (m_Bones[i].GetName() == name) return i;
        return -1;
    }

    void Skeleton::ComputeWorldMatrices(std::vector<Mat4>& out) const
    {
        out.resize(m_Bones.size());

        for (usize i = 0; i < m_Bones.size(); ++i)
        {
            Mat4  local  = m_Bones[i].GetLocalTransform().ToMatrix();
            int32 parent = m_Bones[i].GetParentIndex();

            if (parent >= 0 && parent < static_cast<int32>(out.size()))
                out[i] = out[parent] * local;
            else
                out[i] = local;
        }
    }

} // namespace Good