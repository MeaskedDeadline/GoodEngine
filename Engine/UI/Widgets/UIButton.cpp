#include "UI/Widgets/UIButton.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"

namespace Good
{

    bool UIButton::Draw(const std::string& label, const Vec2& size)
    {
        auto& ctx     = UIContext::Get();
        auto& layout  = UILayout::Get();
        auto& renderer= UIRenderer::Get();
        auto& style   = ctx.GetStyle();

        float32 w = size.x > 0 ? size.x : 120.0f;
        float32 h = size.y > 0 ? size.y : style.metrics.fontSize + style.metrics.itemPadding * 2;

        Rect   rect    = layout.NextItemRect(w, h);
        uint64 id      = ctx.GenerateID(label);
        bool   hovered = ctx.IsItemHovered(rect);
        bool   active  = ctx.IsActive(id);
        bool   clicked = false;

        if (hovered) ctx.SetHovered(id);

        if (hovered && ctx.GetState().mousePressed)
        {
            ctx.SetActive(id);
            active = true;
        }

        if (active && ctx.GetState().mouseReleased && hovered)
        {
            clicked = true;
            ctx.SetActive(0);
        }

        Color bg = active  ? style.colors.buttonActive
                 : hovered ? style.colors.buttonHovered
                 :            style.colors.button;

        renderer.DrawRect(rect, bg, style.metrics.borderRadius);
        renderer.DrawRectOutline(rect, style.colors.border, style.metrics.borderWidth);

        float32 textW = static_cast<float32>(label.size()) * 8.0f;
        Vec2 textPos(
            rect.x + (rect.width  - textW) * 0.5f,
            rect.y + (rect.height - style.metrics.fontSize) * 0.5f);

        renderer.DrawTextUI(label, textPos,
            hovered ? style.colors.textSelected : style.colors.text,
            style.metrics.fontSize);

        return clicked;
    }

    bool UIButton::DrawIcon(const std::string& icon, const std::string& /*tooltip*/)
    {
        return Draw(icon, Vec2(28, 28));
    }

    void UIButton::Render()
    {
        if (!m_Visible) return;
        Draw(m_Label, Vec2(m_Rect.width, m_Rect.height));
    }

} // namespace Good