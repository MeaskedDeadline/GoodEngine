#pragma once
#include "Panel.h"
#include "Scene/Scene.h"

namespace Good
{

    class HierarchyPanel final : public Panel
    {
    public:
        HierarchyPanel();

        void OnRender() override;
        void OnUpdate(float32 dt) override;
        const char* GetTitle() const override { return "Hierarchy"; }

        void SetScene(Ref<Scene> scene) { m_Scene = std::move(scene); }

        Ref<SceneNode> GetSelectedNode() const { return m_Selected; }
        void ClearSelection()                  { m_Selected = nullptr; }

    private:
        void DrawToolbar();
        void DrawNode(const Ref<SceneNode>& node, int32 depth = 0);
        void DrawContextMenu();
        void CreateObject(NodeType type, const std::string& name);
        void DeleteSelected();
        void DuplicateSelected();

        Ref<Scene>     m_Scene;
        Ref<SceneNode> m_Selected;
        std::string    m_SearchText;

        bool    m_ShowContextMenu{false};
        float32 m_ContextMenuX{0};
        float32 m_ContextMenuY{0};
        bool    m_ContextOnNode{false};

        float32 m_ScrollY{0.0f};
        int32   m_NodeCount{0};
    };

} // namespace Good