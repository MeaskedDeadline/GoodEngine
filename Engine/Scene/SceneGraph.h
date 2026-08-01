#pragma once
#include "../Core/Core.h"
#include "SceneNode.h"
#include <functional>

namespace Good
{

    class SceneGraph
    {
    public:
        SceneGraph();

        Ref<SceneNode> GetRoot() const { return m_Root; }

        Ref<SceneNode> CreateNode(const std::string& name);
        Ref<SceneNode> CreateNode(const std::string& name, Ref<SceneNode> parent);

        void DestroyNode(Ref<SceneNode> node);

        Ref<SceneNode> Find(const std::string& name) const;

        void Traverse(const std::function<void(Ref<SceneNode>)>& fn) const;

        usize GetNodeCount() const;
        void  Clear();

    private:
        void TraverseNode(Ref<SceneNode> node,
                          const std::function<void(Ref<SceneNode>)>& fn) const;

        Ref<SceneNode> m_Root;
    };

} // namespace Good