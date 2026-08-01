#pragma once
#include "Core/Core.h"
#include "Math/Transform.h"
#include "Math/Mat4.h"
#include "Math/Color.h"

namespace Good
{

    enum class NodeType : uint8
    {
        Empty = 0,
        Cube,
        Sphere,
        Plane,
        Capsule,
        DirectionalLight,
        PointLight,
        SpotLight,
        Camera
    };

    const char* NodeTypeToString(NodeType type);

    class SceneNode
    {
    public:
        SceneNode();
        explicit SceneNode(const std::string& name);

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName()    const { return m_Name; }

        void SetType(NodeType type)     { m_Type = type; }
        NodeType GetType()        const { return m_Type; }

        void SetColor(const Color& c)   { m_Color = c; }
        const Color& GetColor()   const { return m_Color; }

        Transform&       GetLocalTransform()       { return m_Transform; }
        const Transform& GetLocalTransform() const { return m_Transform; }

        Mat4 GetWorldMatrix()   const;
        Vec3 GetWorldPosition() const;

        void AddChild(Ref<SceneNode> child);
        void RemoveChild(Ref<SceneNode> child);
        void SetParent(SceneNode* parent) { m_Parent = parent; }
        SceneNode* GetParent() const      { return m_Parent; }

        const std::vector<Ref<SceneNode>>& GetChildren() const { return m_Children; }

        bool IsActive() const { return m_Active; }
        void SetActive(bool v){ m_Active = v; }

        Ref<SceneNode> FindChild(const std::string& name) const;
        Ref<SceneNode> FindChildRecursive(const std::string& name) const;
        usize GetTotalChildCount() const;

    private:
        std::string                 m_Name{"Node"};
        NodeType                    m_Type{NodeType::Empty};
        Color                       m_Color{0.7f, 0.7f, 0.8f, 1.0f};
        Transform                   m_Transform;
        SceneNode*                  m_Parent{nullptr};
        std::vector<Ref<SceneNode>> m_Children;
        bool                        m_Active{true};
    };

} // namespace Good