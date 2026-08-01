#include "Panels/ViewportPanel.h"
#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UILayout.h"
#include "UI/Widgets/UIButton.h"
#include "UI/Widgets/UILabel.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"
#include "Platform/Cursor.h"
#include "Platform/Window.h"
#include "Debug/DebugDraw.h"
#include "Core/Time.h"
#include "Core/Log.h"
#include "Graphics/GLFunctions.h"
#include "Math/MathCommon.h"
#include <cstdio>
#include <functional>

namespace Good
{

static const char* g_VS = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aUV;
layout(location=3) in vec4 aColor;
uniform mat4 u_MVP;
uniform mat4 u_Model;
out vec3 vNormal;
void main()
{
    gl_Position = u_MVP * vec4(aPos, 1.0);
    vNormal = mat3(u_Model) * aNormal;
}
)";

static const char* g_FS = R"(
#version 330 core
in vec3 vNormal;
uniform vec4 u_Color;
uniform vec3 u_LightDir;
out vec4 FragColor;
void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(-u_LightDir);
    float diff = max(dot(N, L), 0.0);
    float lighting = 0.35 + diff * 0.75;
    FragColor = vec4(u_Color.rgb * lighting, u_Color.a);
}
)";

static bool    s_Navigating     = false;
static float32 s_CapturedMouseX = 0.0f;
static float32 s_CapturedMouseY = 0.0f;
static float32 s_Yaw            = 0.0f;
static float32 s_Pitch          = 0.0f;
static bool    s_YawPitchInited = false;

ViewportPanel::ViewportPanel()
{
    CameraConfig cfg;
    cfg.fovDegrees = 60.0f;
    cfg.nearPlane  = 0.1f;
    cfg.farPlane   = 1000.0f;
    m_Camera.SetConfig(cfg);
    m_Camera.GetTransform().position = Vec3(4, 3, 8);
    m_Camera.GetTransform().LookAt(Vec3(0, 0, 0));
}

void ViewportPanel::InitializeRenderResources()
{
    if (m_ResourcesInitialized) return;
    m_Shader = MakeRef<Shader>();
    if (!m_Shader->LoadFromSource(g_VS, g_FS)) return;
    m_CubeMesh    = Mesh::CreateCube();
    m_SphereMesh  = Mesh::CreateSphere(16, 16);
    m_PlaneMesh   = Mesh::CreatePlane(1.0f, 1);
    m_CapsuleMesh = Mesh::CreateCapsule(0.5f, 2.0f);
    m_ResourcesInitialized = true;
    GOOD_LOG_INFO("Viewport", "Render resources initialized");
}

Ray ViewportPanel::ScreenToRay(float32 screenX, float32 screenY)
{
    float32 vpX = m_Rect.x;
    float32 vpY = m_Rect.y + 62.0f;
    float32 vpW = m_Rect.width;
    float32 vpH = m_Rect.height - 62.0f;
    if (vpW <= 0 || vpH <= 0) return Ray(Vec3(0,0,0), Vec3(0,0,-1));

    float32 ndcX = ((screenX - vpX) / vpW) * 2.0f - 1.0f;
    float32 ndcY = 1.0f - ((screenY - vpY) / vpH) * 2.0f;

    Mat4 invVP = m_Camera.GetViewProjection().Inverted();
    Vec4 nearP = invVP * Vec4(ndcX, ndcY, -1.0f, 1.0f);
    Vec4 farP  = invVP * Vec4(ndcX, ndcY,  1.0f, 1.0f);
    if (Math::Abs(nearP.w) < 0.0001f || Math::Abs(farP.w) < 0.0001f)
        return Ray(Vec3(0,0,0), Vec3(0,0,-1));

    Vec3 origin(nearP.x/nearP.w, nearP.y/nearP.w, nearP.z/nearP.w);
    Vec3 end(farP.x/farP.w, farP.y/farP.w, farP.z/farP.w);
    return Ray(origin, (end - origin).Normalized());
}

Ref<SceneNode> ViewportPanel::PickObject(float32 screenX, float32 screenY)
{
    if (!m_Scene) return nullptr;
    Ray ray = ScreenToRay(screenX, screenY);
    Ref<SceneNode> closest;
    float32 closestDist = 99999.0f;

    std::function<void(const Ref<SceneNode>&)> check;
    check = [&](const Ref<SceneNode>& node)
    {
        if (!node || !node->IsActive()) return;
        if (node->GetType() == NodeType::Cube || node->GetType() == NodeType::Sphere ||
            node->GetType() == NodeType::Plane || node->GetType() == NodeType::Capsule)
        {
            Vec3 pos  = node->GetWorldPosition();
            Vec3 half = node->GetLocalTransform().scale * 0.5f;
            AABB aabb(pos - half, pos + half);
            RayHit hit;
            if (ray.IntersectsAABB(aabb, hit) && hit.distance < closestDist)
            {
                closestDist = hit.distance;
                closest     = node;
            }
        }
        for (auto& child : node->GetChildren()) check(child);
    };

    auto root = m_Scene->GetRoot();
    for (auto& child : root->GetChildren()) check(child);
    return closest;
}

void ViewportPanel::OnUpdate(float32 dt)
{
    m_IsHovered = UIContext::Get().IsItemHovered(m_Rect);
    auto& input = Input::Get();

    if (!s_YawPitchInited)
    {
        Vec3 euler = m_Camera.GetTransform().rotation.ToEuler();
        s_Pitch = euler.x;
        s_Yaw   = euler.y;
        s_YawPitchInited = true;
    }

    float32 vpCenterX = m_Rect.x + m_Rect.width  * 0.5f;
    float32 vpCenterY = m_Rect.y + m_Rect.height * 0.5f;

    // ═══ 1. Конец drag ═══
    if (m_DraggingGizmo && !input.IsMouseButtonDown(MouseButton::Left))
    {
        m_DraggingGizmo = false;
        m_DragAxis = GizmoAxis::None;
        m_TranslateGizmo.SetActiveAxis(GizmoAxis::None);
        m_ScaleGizmo.SetActiveAxis(GizmoAxis::None);
        m_RotateGizmo.SetActiveAxis(GizmoAxis::None);
    }

    // ═══ 2. Продолжение drag ═══
    if (m_DraggingGizmo && m_SelectedNode)
    {
        Ray currentRay = ScreenToRay(input.GetMouseX(), input.GetMouseY());
        Vec3 axisDir = Gizmo::GetAxisDirection(m_DragAxis);

        Vec3 w = currentRay.origin - m_DragStartObjPos;
        float32 a = currentRay.direction.Dot(currentRay.direction);
        float32 b = currentRay.direction.Dot(axisDir);
        float32 c = axisDir.Dot(axisDir);
        float32 d = currentRay.direction.Dot(w);
        float32 e = axisDir.Dot(w);
        float32 denom = a * c - b * b;

        if (Math::Abs(denom) > Math::EPSILON)
        {
            float32 t = (a * e - b * d) / denom;
            float32 delta = t - m_DragStartT;

            if (m_GizmoMode == GizmoMode::Translate)
            {
                m_SelectedNode->GetLocalTransform().position =
                    m_DragStartObjPos + axisDir * delta;
            }
            else if (m_GizmoMode == GizmoMode::Scale)
            {
                float32 factor = 1.0f + delta * 0.5f;
                if (factor < 0.05f) factor = 0.05f;

                Vec3 newScale = m_DragStartScale;
                if (m_DragAxis == GizmoAxis::X) newScale.x = m_DragStartScale.x * factor;
                if (m_DragAxis == GizmoAxis::Y) newScale.y = m_DragStartScale.y * factor;
                if (m_DragAxis == GizmoAxis::Z) newScale.z = m_DragStartScale.z * factor;

                m_SelectedNode->GetLocalTransform().scale = newScale;
            }
            else if (m_GizmoMode == GizmoMode::Rotate)
            {
                float32 angleDeg = delta * 30.0f;

                Vec3 euler = m_DragStartRotEuler;
                if (m_DragAxis == GizmoAxis::X) euler.x += angleDeg;
                if (m_DragAxis == GizmoAxis::Y) euler.y += angleDeg;
                if (m_DragAxis == GizmoAxis::Z) euler.z += angleDeg;

                m_SelectedNode->GetLocalTransform().rotation =
                    Quat::FromEuler(euler.x, euler.y, euler.z);
            }
        }
    }

    // ═══ 3. ЛКМ нажата ═══
    if (m_IsHovered && !s_Navigating && !m_DraggingGizmo &&
        input.IsMouseButtonPressed(MouseButton::Left))
    {
        Ray mouseRay = ScreenToRay(input.GetMouseX(), input.GetMouseY());
        bool startedDrag = false;

        if (m_SelectedNode)
        {
            Vec3 objPos = m_SelectedNode->GetWorldPosition();
            GizmoAxis hitAxis = GizmoAxis::None;

            if (m_GizmoMode == GizmoMode::Translate)
                hitAxis = m_TranslateGizmo.HitTest(objPos, mouseRay, 0.5f);
            else if (m_GizmoMode == GizmoMode::Scale)
                hitAxis = m_ScaleGizmo.HitTest(objPos, mouseRay, 0.5f);
            else if (m_GizmoMode == GizmoMode::Rotate)
                hitAxis = m_RotateGizmo.HitTest(objPos, mouseRay, 0.5f);

            if (hitAxis != GizmoAxis::None)
            {
                m_DraggingGizmo     = true;
                m_DragAxis          = hitAxis;
                m_DragStartObjPos   = objPos;
                m_DragStartScale    = m_SelectedNode->GetLocalTransform().scale;
                m_DragStartRotEuler = m_SelectedNode->GetLocalTransform().rotation.ToEuler();
                startedDrag         = true;

                if (m_GizmoMode == GizmoMode::Translate)
                    m_TranslateGizmo.SetActiveAxis(hitAxis);
                else if (m_GizmoMode == GizmoMode::Scale)
                    m_ScaleGizmo.SetActiveAxis(hitAxis);
                else if (m_GizmoMode == GizmoMode::Rotate)
                    m_RotateGizmo.SetActiveAxis(hitAxis);

                Vec3 axisDir = Gizmo::GetAxisDirection(hitAxis);
                Vec3 w = mouseRay.origin - objPos;
                float32 a = mouseRay.direction.Dot(mouseRay.direction);
                float32 b = mouseRay.direction.Dot(axisDir);
                float32 c = axisDir.Dot(axisDir);
                float32 d = mouseRay.direction.Dot(w);
                float32 e = axisDir.Dot(w);
                float32 denom = a * c - b * b;
                m_DragStartT = (Math::Abs(denom) > Math::EPSILON)
                    ? (a * e - b * d) / denom : 0.0f;
            }
        }

        if (!startedDrag)
            m_SelectedNode = PickObject(input.GetMouseX(), input.GetMouseY());
    }

    // ═══ 4. Горячие клавиши ═══
    if (m_IsHovered && !s_Navigating && !m_DraggingGizmo)
    {
        if (UIContext::Get().GetState().focusedID == 0)
        {
            if (input.IsKeyPressed(Key::W)) m_GizmoMode = GizmoMode::Translate;
            if (input.IsKeyPressed(Key::E)) m_GizmoMode = GizmoMode::Rotate;
            if (input.IsKeyPressed(Key::R)) m_GizmoMode = GizmoMode::Scale;
        }

        if (input.IsKeyPressed(Key::F) && m_SelectedNode)
        {
            Vec3 target = m_SelectedNode->GetWorldPosition();
            Vec3 camPos = target + Vec3(0, 2, 4);
            m_Camera.GetTransform().position = camPos;
            Vec3 dir = (target - camPos).Normalized();
            s_Pitch = Math::ToDegrees(Math::Asin(-dir.y));
            s_Yaw   = Math::ToDegrees(Math::Atan2(-dir.x, -dir.z));
            m_Camera.GetTransform().rotation = Quat::FromEuler(s_Pitch, s_Yaw, 0.0f);
        }
    }

    // ═══ 5. Навигация ═══
    if (m_IsHovered && input.IsMouseButtonPressed(MouseButton::Right) &&
        !s_Navigating && !m_DraggingGizmo)
    {
        s_Navigating = true;
        s_CapturedMouseX = input.GetMouseX();
        s_CapturedMouseY = input.GetMouseY();
        Cursor::SetVisible(false);
        Cursor::SetPosition(vpCenterX, vpCenterY);
        input.OnMouseMove(vpCenterX, vpCenterY);
        input.ResetMouseDelta();
    }

    if (s_Navigating && !input.IsMouseButtonDown(MouseButton::Right))
    {
        s_Navigating = false;
        Cursor::SetPosition(s_CapturedMouseX, s_CapturedMouseY);
        input.OnMouseMove(s_CapturedMouseX, s_CapturedMouseY);
        input.ResetMouseDelta();
        Cursor::SetVisible(true);
    }

    if (s_Navigating)
    {
        UIContext::Get().GetState().hoveredID     = 0;
        UIContext::Get().GetState().mouseDown     = false;
        UIContext::Get().GetState().mousePressed  = false;
        UIContext::Get().GetState().mouseReleased = false;

        float32 mx = input.GetMouseX();
        float32 my = input.GetMouseY();
        float32 dx = mx - vpCenterX;
        float32 dy = my - vpCenterY;

        const float32 DEADZONE = 1.5f;
        if (Math::Abs(dx) > DEADZONE || Math::Abs(dy) > DEADZONE)
        {
            if (Math::Abs(dx) <= DEADZONE) dx = 0.0f;
            if (Math::Abs(dy) <= DEADZONE) dy = 0.0f;
            s_Yaw   -= dx * 0.15f;
            s_Pitch -= dy * 0.15f;
            s_Pitch  = Math::Clamp(s_Pitch, -89.0f, 89.0f);
        }

        Transform& tr = m_Camera.GetTransform();
        tr.rotation = Quat::FromEuler(s_Pitch, s_Yaw, 0.0f);

        bool fast = input.IsKeyDown(Key::LeftShift) ||
                    input.IsKeyDown(static_cast<Key>(16));
        float32 speed = fast ? 15.0f : 5.0f;

        Vec3 fwd   = tr.Forward();
        Vec3 right = tr.Right();
        Vec3 up(0, 1, 0);

        if (input.IsKeyDown(Key::W)) tr.position += fwd   * speed * dt;
        if (input.IsKeyDown(Key::S)) tr.position -= fwd   * speed * dt;
        if (input.IsKeyDown(Key::D)) tr.position += right * speed * dt;
        if (input.IsKeyDown(Key::A)) tr.position -= right * speed * dt;
        if (input.IsKeyDown(Key::E)) tr.position += up    * speed * dt;
        if (input.IsKeyDown(Key::Q)) tr.position -= up    * speed * dt;

        Cursor::SetPosition(vpCenterX, vpCenterY);
        input.OnMouseMove(vpCenterX, vpCenterY);
    }

    if (m_Rect.height > 0)
        m_Camera.SetAspect(m_Rect.width / m_Rect.height);
}

void ViewportPanel::DrawToolbar()
{
    auto& renderer = UIRenderer::Get();
    Rect bar(m_Rect.x, m_Rect.y + 28, m_Rect.width, 34.0f);
    renderer.DrawRect(bar, Color(0.10f, 0.10f, 0.13f, 1.0f));

    float32 y = m_Rect.y + 32;
    float32 x = m_Rect.x + 8;

    auto drawBtn = [&](const char* label, float32 w) -> bool
    {
        UILayout::Get().PushLayout(Rect(x, y, w, 26));
        bool clicked = UIButton::Draw(label, Vec2(w, 26));
        UILayout::Get().PopLayout();
        x += w + 3;
        return clicked;
    };

    if (drawBtn("Move",  55)) m_GizmoMode = GizmoMode::Translate;
    if (drawBtn("Rot",   45)) m_GizmoMode = GizmoMode::Rotate;
    if (drawBtn("Scale", 60)) m_GizmoMode = GizmoMode::Scale;
    x += 12;

    bool isWorld = (m_GizmoSpace == GizmoSpace::World);
    if (drawBtn(isWorld ? "World" : "Local", 60))
        m_GizmoSpace = isWorld ? GizmoSpace::Local : GizmoSpace::World;
    x += 12;

    if (drawBtn(m_PlayMode ? "Pause" : "Play", 55))
        m_PlayMode = !m_PlayMode;
    if (drawBtn("Stop", 50))
        m_PlayMode = false;
}

void ViewportPanel::RenderNode(const Ref<SceneNode>& node)
{
    if (!node || !node->IsActive()) return;

    Ref<Mesh> mesh;
    switch (node->GetType())
    {
        case NodeType::Cube:    mesh = m_CubeMesh;    break;
        case NodeType::Sphere:  mesh = m_SphereMesh;  break;
        case NodeType::Plane:   mesh = m_PlaneMesh;   break;
        case NodeType::Capsule: mesh = m_CapsuleMesh; break;
        default: break;
    }

    if (mesh && m_Shader && m_Shader->IsValid())
    {
        Mat4 model = node->GetWorldMatrix();
        Mat4 mvp   = m_Camera.GetViewProjection() * model;
        m_Shader->Bind();
        m_Shader->SetMat4("u_MVP",      mvp);
        m_Shader->SetMat4("u_Model",    model);
        m_Shader->SetColor("u_Color",   node->GetColor());
        m_Shader->SetVec3("u_LightDir", Vec3(-0.5f, -1.0f, -0.3f).Normalized());
        mesh->Draw();
        m_Shader->Unbind();
    }

    switch (node->GetType())
    {
        case NodeType::DirectionalLight:
        {
            Vec3 pos = node->GetWorldPosition();
            Vec3 dir = node->GetLocalTransform().Forward();
            DebugDraw::Line(pos, pos + dir * 1.5f, Color::Yellow());
            DebugDraw::Sphere(pos, 0.15f, Color::Yellow());
            break;
        }
        case NodeType::PointLight:
            DebugDraw::Sphere(node->GetWorldPosition(), 0.2f, Color::Yellow());
            break;
        case NodeType::Camera:
            DebugDraw::Box(node->GetWorldPosition(), Vec3(0.3f, 0.3f, 0.5f), Color::Cyan());
            break;
        default: break;
    }

    for (auto& child : node->GetChildren())
        RenderNode(child);
}

void ViewportPanel::RenderScene()
{
    InitializeRenderResources();
    if (m_Rect.height > 0) m_Camera.SetAspect(m_Rect.width / m_Rect.height);
    if (m_Rect.width < 10 || m_Rect.height < 10) return;

    float32 topOffset = 62.0f;
    int32 vpX = static_cast<int32>(m_Rect.x);
    int32 vpW = static_cast<int32>(m_Rect.width);
    int32 vpH = static_cast<int32>(m_Rect.height - topOffset);
    uint32 fullH = UIContext::Get().GetViewportH();
    int32 vpY = static_cast<int32>(fullH - m_Rect.y - m_Rect.height);
    if (vpW <= 0 || vpH <= 0) return;

    glEnable(GL_SCISSOR_TEST);
    glScissor(vpX, vpY, vpW, vpH);
    glViewport(vpX, vpY, vpW, vpH);
    glClearColor(0.10f, 0.10f, 0.13f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    auto& renderer = Renderer::Get();
    renderer.BeginScene(m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix());

    DebugDraw::Grid(Vec3(0,0,0), 20.0f, 20, Color(0.25f, 0.25f, 0.28f, 1.0f));
    DebugDraw::Axes(Mat4::Identity(), 0.5f);

    if (m_Scene)
    {
        auto root = m_Scene->GetRoot();
        for (auto& child : root->GetChildren()) RenderNode(child);
    }

    if (m_SelectedNode)
    {
        Vec3 pos = m_SelectedNode->GetWorldPosition();
        Mat4 vp  = m_Camera.GetViewProjection();

        switch (m_GizmoMode)
        {
            case GizmoMode::Translate: m_TranslateGizmo.Draw(pos, vp); break;
            case GizmoMode::Rotate:    m_RotateGizmo.Draw(pos, vp);    break;
            case GizmoMode::Scale:     m_ScaleGizmo.Draw(pos, vp);     break;
            default: break;
        }

        Vec3 scale = m_SelectedNode->GetLocalTransform().scale;
        DebugDraw::Box(pos, scale * 1.1f, Color(1.0f, 0.6f, 0.0f, 1.0f));
    }

    DebugDraw::Flush(m_Camera.GetViewProjection());
    DebugDraw::Update(Time::DeltaTime());
    renderer.EndScene();

    uint32 fullW = UIContext::Get().GetViewportW();
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, static_cast<GLsizei>(fullW), static_cast<GLsizei>(fullH));
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void ViewportPanel::OnRender()
{
    if (!m_Visible) return;
    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();

    renderer.DrawRectOutline(m_Rect, style.colors.border, 1.0f);

    Rect headerRect(m_Rect.x, m_Rect.y, m_Rect.width, 28.0f);
    renderer.DrawRect(headerRect, style.colors.titleBar);
    renderer.DrawTextUI("VIEWPORT", Vec2(m_Rect.x + 12, m_Rect.y + 7),
        style.colors.text, 14.0f);

    DrawToolbar();

    Vec3 camPos = m_Camera.GetTransform().position;
    char camStr[64];
    std::snprintf(camStr, sizeof(camStr), "Camera  X %.1f  Y %.1f  Z %.1f",
        static_cast<double>(camPos.x), static_cast<double>(camPos.y),
        static_cast<double>(camPos.z));

    renderer.DrawTextUI(camStr,
        Vec2(m_Rect.x + 12, m_Rect.y + m_Rect.height - 22),
        style.colors.textDisabled, 12.0f);

    renderer.DrawTextUI("W=Move E=Rot R=Scale F=Focus LMB=Select/Drag",
        Vec2(m_Rect.x + m_Rect.width - 420, m_Rect.y + m_Rect.height - 22),
        style.colors.textDisabled, 11.0f);
}

} // namespace Good