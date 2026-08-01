#include "UI/Widgets/UILabel.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"

namespace Good
{

    bool UILabel::Draw(const std::string& text, const Vec2& pos)
    {
        auto& ctx      = UIContext::Get();
        auto& renderer = UIRenderer::Get();
        auto& style    = ctx.GetStyle();

        Rect rect = UILayout::Get().NextItemRect(200, style.metrics.fontSize + 4);
        Vec2 p    = (pos.x == 0 && pos.y == 0) ? Vec2(rect.x, rect.y) : pos;

        renderer.DrawTextUI(text, p, style.colors.text, style.metrics.fontSize);
        return true;
    }

    bool UILabel::DrawColored(const std::string& text, const Color& color, const Vec2& pos)
    {
        auto& ctx      = UIContext::Get();
        auto& renderer = UIRenderer::Get();
        auto& style    = ctx.GetStyle();

        Rect rect = UILayout::Get().NextItemRect(200, style.metrics.fontSize + 4);
        Vec2 p    = (pos.x == 0 && pos.y == 0) ? Vec2(rect.x, rect.y) : pos;

        renderer.DrawTextUI(text, p, color, style.metrics.fontSize);
        return true;
    }

    void UILabel::Render()
    {
        if (!m_Visible) return;
        UIRenderer::Get().DrawTextUI(m_Text,
            Vec2(m_Rect.x, m_Rect.y),
            UIContext::Get().GetStyle().colors.text,
            UIContext::Get().GetStyle().metrics.fontSize);
    }

} // namespace Good