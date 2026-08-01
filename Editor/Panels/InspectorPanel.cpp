#include "Panels/InspectorPanel.h"
#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UILayout.h"
#include "UI/Widgets/UILabel.h"
#include "UI/Widgets/UIButton.h"
#include "UI/Widgets/UIFloatField.h"
#include "UI/Widgets/UICheckbox.h"
#include "UI/Widgets/UITextInput.h"

namespace Good
{

static float32 s_RotX = 0;
static float32 s_RotY = 0;
static float32 s_RotZ = 0;
static Transform* s_LastTransform = nullptr;

InspectorPanel::InspectorPanel() = default;

void InspectorPanel::DrawTransform(Transform& transform)
{
    auto& style    = UIContext::Get().GetStyle();
    auto& renderer = UIRenderer::Get();

    Vec2 c = UILayout::Get().GetCursor();
    Rect sectionHeader(c.x, c.y, m_Rect.width - 24, 22);
    renderer.DrawRect(sectionHeader, style.colors.header);
    renderer.DrawTextUI("TRANSFORM",
        Vec2(sectionHeader.x + 6, sectionHeader.y + 5),
        style.colors.text, 12.0f);
    UILayout::Get().Spacing(28);

    UILabel::DrawColored("Position", Color(0.7f, 0.7f, 0.7f, 1.0f));
    UIFloatField::Draw("X", transform.position.x, 0.05f, 90.0f);
    UIFloatField::Draw("Y", transform.position.y, 0.05f, 90.0f);
    UIFloatField::Draw("Z", transform.position.z, 0.05f, 90.0f);
    UILayout::Get().Spacing(8);

    UILabel::DrawColored("Rotation", Color(0.7f, 0.7f, 0.7f, 1.0f));

    if (&transform != s_LastTransform)
    {
        Vec3 euler = transform.rotation.ToEuler();
        s_RotX = euler.x;
        s_RotY = euler.y;
        s_RotZ = euler.z;
        s_LastTransform = &transform;
    }

    bool rotChanged = false;
    rotChanged |= UIFloatField::Draw("X", s_RotX, 0.5f, 90.0f);
    rotChanged |= UIFloatField::Draw("Y", s_RotY, 0.5f, 90.0f);
    rotChanged |= UIFloatField::Draw("Z", s_RotZ, 0.5f, 90.0f);

    if (rotChanged)
        transform.rotation = Quat::FromEuler(s_RotX, s_RotY, s_RotZ);

    UILayout::Get().Spacing(8);

    UILabel::DrawColored("Scale", Color(0.7f, 0.7f, 0.7f, 1.0f));
    UIFloatField::Draw("X", transform.scale.x, 0.02f, 90.0f);
    UIFloatField::Draw("Y", transform.scale.y, 0.02f, 90.0f);
    UIFloatField::Draw("Z", transform.scale.z, 0.02f, 90.0f);
    UILayout::Get().Spacing(12);
}

void InspectorPanel::DrawNodeInfo(const Ref<SceneNode>& node)
{
    auto& style = UIContext::Get().GetStyle();

    std::string name = node->GetName();
    if (UITextInput::Draw("Name", name, "", m_Rect.width - 90))
        node->SetName(name);
    UILayout::Get().Spacing(6);

    bool active = node->IsActive();
    if (UICheckbox::Draw("Active", active))
        node->SetActive(active);
    UILayout::Get().Spacing(12);

    DrawTransform(node->GetLocalTransform());

    std::string info = "Children: " + std::to_string(node->GetChildren().size());
    UILabel::DrawColored(info, style.colors.textDisabled);
}

void InspectorPanel::DrawObjectInfo(const Ref<GameObject>& obj)
{
    std::string name = obj->GetName();
    if (UITextInput::Draw("Name", name, "", m_Rect.width - 90))
        obj->SetName(name);
    UILayout::Get().Spacing(6);

    bool active = obj->IsActive();
    if (UICheckbox::Draw("Active", active))
        obj->SetActive(active);
    UILayout::Get().Spacing(12);

    DrawTransform(obj->GetTransform());
}

void InspectorPanel::DrawAddComponentButton()
{
    UILayout::Get().Spacing(12);

    float32 btnW    = 200.0f;
    float32 centerX = m_Rect.x + (m_Rect.width - btnW) * 0.5f;
    Vec2 cur = UILayout::Get().GetCursor();

    UILayout::Get().PushLayout(Rect(centerX, cur.y, btnW, 28));
    UIButton::Draw("+ Add Component", Vec2(btnW, 28));
    UILayout::Get().PopLayout();
}

void InspectorPanel::OnRender()
{
    if (!m_Visible) return;

    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();

    renderer.DrawRect(m_Rect, style.colors.windowBackground);
    renderer.DrawRectOutline(m_Rect, style.colors.border, 1.0f);

    Rect headerRect(m_Rect.x, m_Rect.y, m_Rect.width, 28.0f);
    renderer.DrawRect(headerRect, style.colors.titleBar);
    renderer.DrawTextUI("INSPECTOR",
        Vec2(m_Rect.x + 12, m_Rect.y + 7),
        style.colors.text, 14.0f);

    Rect content(m_Rect.x + 10, m_Rect.y + 38,
                 m_Rect.width - 20, m_Rect.height - 48);
    UILayout::Get().PushLayout(content);

    if (m_SelectedNode)
    {
        DrawNodeInfo(m_SelectedNode);
        DrawAddComponentButton();
    }
    else if (m_SelectedObject)
    {
        DrawObjectInfo(m_SelectedObject);
        DrawAddComponentButton();
    }
    else
    {
        s_LastTransform = nullptr;
        UILabel::DrawColored("No selection", style.colors.textDisabled);
        UILayout::Get().Spacing(8);
        UILabel::DrawColored("Select object from Hierarchy",
                             style.colors.textDisabled);
    }

    UILayout::Get().PopLayout();
}

} // namespace Good