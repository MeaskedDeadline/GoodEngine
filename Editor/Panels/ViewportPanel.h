#pragma once
#include "Panel.h"
#include "Camera/Camera.h"
#include "Camera/FreeCamera.h"
#include "Rendering/Renderer.h"
#include "Rendering/Mesh.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Shader.h"
#include "Scene/Scene.h"
#include "Scene/SceneNode.h"
#include "Physics/AABB.h"
#include "Physics/Ray.h"
#include "Math/Vec4.h"
#include "Gizmos/Gizmo.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"

namespace Good
{

class ViewportPanel final : public Panel
{
public:
    ViewportPanel();

    void OnRender() override;
    void OnUpdate(float32 dt) override;
    const char* GetTitle() const override { return "Viewport"; }

    Camera& GetCamera() { return m_Camera; }

    void SetGizmoMode(GizmoMode mode)   { m_GizmoMode  = mode; }
    void SetGizmoSpace(GizmoSpace sp)   { m_GizmoSpace = sp;   }
    GizmoMode GetGizmoMode() const      { return m_GizmoMode;  }

    void SetSelectedNode(Ref<SceneNode> node) { m_SelectedNode = std::move(node); }
    Ref<SceneNode> GetSelectedNode() const    { return m_SelectedNode; }
    void SetScene(Ref<Scene> scene)           { m_Scene = std::move(scene); }

    void RenderScene();

private:
    void DrawToolbar();
    void RenderNode(const Ref<SceneNode>& node);
    void InitializeRenderResources();
    Ref<SceneNode> PickObject(float32 screenX, float32 screenY);
    Ray ScreenToRay(float32 screenX, float32 screenY);

    Camera         m_Camera;
    FreeCamera     m_FreeCam;
    Framebuffer    m_Framebuffer;

    GizmoMode      m_GizmoMode{GizmoMode::Translate};
    GizmoSpace     m_GizmoSpace{GizmoSpace::World};

    TranslateGizmo m_TranslateGizmo;
    RotateGizmo    m_RotateGizmo;
    ScaleGizmo     m_ScaleGizmo;

    Ref<Scene>     m_Scene;
    Ref<SceneNode> m_SelectedNode;

    Ref<Shader>    m_Shader;
    Ref<Mesh>      m_CubeMesh;
    Ref<Mesh>      m_SphereMesh;
    Ref<Mesh>      m_PlaneMesh;
    Ref<Mesh>      m_CapsuleMesh;
    bool           m_ResourcesInitialized{false};

    bool m_IsHovered{false};
    bool m_PlayMode{false};

    bool      m_DraggingGizmo{false};
    GizmoAxis m_DragAxis{GizmoAxis::None};
    Vec3      m_DragStartObjPos{0,0,0};
    Vec3      m_DragStartScale{1,1,1};
    Vec3      m_DragStartRotEuler{0,0,0};
    float32   m_DragStartT{0.0f};
};

} // namespace Good