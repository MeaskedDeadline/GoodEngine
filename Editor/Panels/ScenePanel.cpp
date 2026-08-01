#include "Panels/ScenePanel.h"
#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UILayout.h"
#include "UI/Widgets/UILabel.h"
#include "UI/Widgets/UIButton.h"
#include "UI/Widgets/UITextInput.h"

namespace Good
{

    ScenePanel::ScenePanel() = default;

    void ScenePanel::OnRender()
    {
        if (!m_Visible) return;

        auto& renderer = UIRenderer::Get();
        auto& style    = UIContext::Get().GetStyle();

        renderer.DrawRect(m_Rect, style.colors.windowBackground);
        renderer.DrawRectOutline(m_Rect, style.colors.border, 1.0f);

        Rect headerRect(m_Rect.x, m_Rect.y, m_Rect.width, 28.0f);
        renderer.DrawRect(headerRect, style.colors.titleBar);
        renderer.DrawTextUI("SCENE",
            Vec2(m_Rect.x + 12, m_Rect.y + 7),
            style.colors.text, 14.0f);

        Rect content(m_Rect.x + 8, m_Rect.y + 38,
                     m_Rect.width - 16, m_Rect.height - 46);
        UILayout::Get().PushLayout(content);

        UILabel::Draw("Scenes:");
        UILayout::Get().Spacing(6);

        UITextInput::Draw("", m_NewSceneName, "Scene name...", 180);
        UILayout::Get().SameLine(4);

        if (UIButton::Draw("New", Vec2(50, 22)) && !m_NewSceneName.empty())
            SceneManager::Get().CreateScene(m_NewSceneName);

        UILayout::Get().Spacing(12);

        auto scene = SceneManager::Get().GetActiveScene();
        if (scene)
        {
            UILabel::DrawColored("Active: " + scene->GetName(),
                Color(0.6f, 0.9f, 0.6f, 1.0f));
            UILabel::DrawColored("Nodes: " + std::to_string(scene->GetNodeCount()),
                style.colors.textDisabled);
        }
        else
        {
            UILabel::DrawColored("No active scene", style.colors.textDisabled);
        }

        UILayout::Get().PopLayout();
    }

} // namespace Good