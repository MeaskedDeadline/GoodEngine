#include "Bone.h"

namespace Good
{

    Bone::Bone(const std::string& name, int32 parentIndex)
        : m_Name(name), m_ParentIndex(parentIndex) {}

} // namespace Good