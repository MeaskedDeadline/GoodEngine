#pragma once
#include "Core/Core.h"
#include "Bone.h"
#include "Math/Mat4.h"

namespace Good
{

    class Skeleton
    {
    public:
        Skeleton() = default;

        void  AddBone(const Bone& bone);
        int32 FindBone(const std::string& name) const;

        Bone&       GetBone(int32 index)       { return m_Bones[index]; }
        const Bone& GetBone(int32 index) const { return m_Bones[index]; }
        usize       GetBoneCount()       const { return m_Bones.size(); }

        void ComputeWorldMatrices(std::vector<Mat4>& outMatrices) const;

    private:
        std::vector<Bone> m_Bones;
    };

} // namespace Good