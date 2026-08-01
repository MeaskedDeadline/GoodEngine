#include "SceneNode.h"

namespace Good
{

const char* NodeTypeToString(NodeType type)
{
    switch (type)
    {
        case NodeType::Empty:            return "Empty";
        case NodeType::Cube:             return "Cube";
        case NodeType::Sphere:           return "Sphere";
        case NodeType::Plane:            return "Plane";
        case NodeType::Capsule:          return "Capsule";
        case NodeType::DirectionalLight: return "Directional Light";
        case NodeType::PointLight:       return "Point Light";
        case NodeType::SpotLight:        return "Spot Light";
        case NodeType::Camera:           return "Camera";
        default:                         return "Unknown";
    }
}

SceneNode::SceneNode() = default;
SceneNode::SceneNode(const std::string& name) : m_Name(name) {}

Mat4 SceneNode::GetWorldMatrix() const
{
    Mat4 local = m_Transform.ToMatrix();
    return m_Parent ? m_Parent->GetWorldMatrix() * local : local;
}

Vec3 SceneNode::GetWorldPosition() const
{
    return GetWorldMatrix().GetTranslation();
}

void SceneNode::AddChild(Ref<SceneNode> child)
{
    if (!child) return;
    child->m_Parent = this;
    m_Children.push_back(std::move(child));
}

void SceneNode::RemoveChild(Ref<SceneNode> child)
{
    if (!child) return;
    for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
    {
        if (*it == child)
        {
            (*it)->m_Parent = nullptr;
            m_Children.erase(it);
            return;
        }
    }
}

Ref<SceneNode> SceneNode::FindChild(const std::string& name) const
{
    for (auto& c : m_Children) if (c->m_Name == name) return c;
    return nullptr;
}

Ref<SceneNode> SceneNode::FindChildRecursive(const std::string& name) const
{
    for (auto& c : m_Children)
    {
        if (c->m_Name == name) return c;
        auto found = c->FindChildRecursive(name);
        if (found) return found;
    }
    return nullptr;
}

usize SceneNode::GetTotalChildCount() const
{
    usize count = m_Children.size();
    for (auto& c : m_Children) count += c->GetTotalChildCount();
    return count;
}

} // namespace Good