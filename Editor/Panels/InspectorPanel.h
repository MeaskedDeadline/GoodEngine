#pragma once
#include "Panel.h"
#include "Scene/SceneNode.h"
#include "Objects/GameObject.h"

namespace Good
{

    class InspectorPanel final : public Panel
    {
    public:
        InspectorPanel();

        void OnRender() override;
        const char* GetTitle() const override { return "Inspector"; }

        void SetSelectedNode(Ref<SceneNode> node)   { m_SelectedNode   = std::move(node); }
        void SetSelectedObject(Ref<GameObject> obj) { m_SelectedObject = std::move(obj); }
        void ClearSelection()
        {
            m_SelectedNode   = nullptr;
            m_SelectedObject = nullptr;
        }

    private:
        void DrawTransform(Transform& transform);
        void DrawNodeInfo(const Ref<SceneNode>& node);
        void DrawObjectInfo(const Ref<GameObject>& obj);
        void DrawAddComponentButton();

        Ref<SceneNode>  m_SelectedNode;
        Ref<GameObject> m_SelectedObject;
    };

} // namespace Good