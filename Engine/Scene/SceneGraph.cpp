#include "SceneGraph.h"

namespace Good
{

    SceneGraph::SceneGraph()
    {
        m_Root = MakeRef<SceneNode>("Root");
    }

    Ref<SceneNode> SceneGraph::CreateNode(const std::string& name)
    {
        auto node = MakeRef<SceneNode>(name);
        m_Root->AddChild(node);
        return node;
    }

    Ref<SceneNode> SceneGraph::CreateNode(const std::string& name, Ref<SceneNode> parent)
    {
        auto node = MakeRef<SceneNode>(name);
        if (parent) parent->AddChild(node);
        else        m_Root->AddChild(node);
        return node;
    }

    void SceneGraph::DestroyNode(Ref<SceneNode> node)
    {
        if (!node || !node->GetParent()) return;
        Ref<SceneNode> parentRef = nullptr;
        Traverse([&](Ref<SceneNode> n)
        {
            for (auto& c : n->GetChildren())
            {
                if (c == node) { parentRef = n; return; }
            }
        });
        if (parentRef) parentRef->RemoveChild(node);
    }

    Ref<SceneNode> SceneGraph::Find(const std::string& name) const
    {
        return m_Root->FindChildRecursive(name);
    }

    void SceneGraph::Traverse(const std::function<void(Ref<SceneNode>)>& fn) const
    {
        TraverseNode(m_Root, fn);
    }

    void SceneGraph::TraverseNode(Ref<SceneNode> node,
                                   const std::function<void(Ref<SceneNode>)>& fn) const
    {
        if (!node) return;
        fn(node);
        for (auto& child : node->GetChildren())
            TraverseNode(child, fn);
    }

    usize SceneGraph::GetNodeCount() const { return m_Root->GetTotalChildCount(); }
    void  SceneGraph::Clear()              { m_Root = MakeRef<SceneNode>("Root"); }

} // namespace Good