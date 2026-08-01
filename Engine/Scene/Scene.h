#pragma once
#include "../Core/Core.h"
#include "SceneGraph.h"

namespace Good
{

    class Scene
    {
    public:
        Scene();
        explicit Scene(const std::string& name);
        ~Scene();

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName()    const { return m_Name; }

        SceneGraph&       GetGraph()       { return m_Graph; }
        const SceneGraph& GetGraph() const { return m_Graph; }

        Ref<SceneNode> GetRoot() const { return m_Graph.GetRoot(); }

        Ref<SceneNode> CreateNode(const std::string& name);
        Ref<SceneNode> CreateNode(const std::string& name, Ref<SceneNode> parent);
        Ref<SceneNode> FindNode(const std::string& name) const;

        void Update(float32 dt);
        void Clear();

        usize GetNodeCount() const { return m_Graph.GetNodeCount(); }

    private:
        std::string m_Name{"Untitled"};
        SceneGraph  m_Graph;
    };

} // namespace Good