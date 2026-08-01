#include "UI/Widgets/UICheckbox.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"

namespace Good
{

bool UICheckbox::Draw(const std::string& label, bool& value)
{
    auto& ctx     = UIContext::Get();
    auto& layout  = UILayout::Get();
    auto& renderer= UIRenderer::Get();
    auto& style   = ctx.GetStyle();

    float32 size = style.metrics.checkboxSize;
    float32 h    = size + style.metrics.itemPadding;
    float32 w    = size + style.metrics.itemSpacing +
                   static_cast<float32>(label.size()) * 8.0f + 16.0f;

    Rect   rect    = layout.NextItemRect(w, h);
    uint64 id      = ctx.GenerateID(label);
    Rect   boxRect(rect.x, rect.y + (h - size) * 0.5f, size, size);

    bool hovered = ctx.IsItemHovered(boxRect);
    bool clicked = false;

    if (hovered) ctx.SetHovered(id);

    if (hovered && ctx.GetState().mousePressed)
    {
        value   = !value;
        clicked = true;
    }

    Color bg = value ? style.colors.checkboxOn
             : hovered ? style.colors.backgroundHovered
             :            style.colors.checkboxOff;

    renderer.DrawRect(boxRect, bg, 3.0f);
    renderer.DrawRectOutline(boxRect, style.colors.border, 1.0f);

    if (value)
    {
        float32 pad = 3.0f;
        renderer.DrawLine(
            Vec2(boxRect.x + pad,         boxRect.y + size * 0.6f),
            Vec2(boxRect.x + size * 0.4f, boxRect.y + size - pad),
            Color::White(), 2.0f);
        renderer.DrawLine(
            Vec2(boxRect.x + size * 0.4f, boxRect.y + size - pad),
            Vec2(boxRect.x + size - pad,  boxRect.y + pad),
            Color::White(), 2.0f);
    }

    renderer.DrawTextUI(label,
        Vec2(rect.x + size + style.metrics.itemSpacing + 2,
             rect.y + (h - style.metrics.fontSize) * 0.5f),
        style.colors.text, style.metrics.fontSize);

    return clicked;
}

void UICheckbox::Render()
{
    if (!m_Visible || !m_Value) return;
    Draw(m_Label, *m_Value);
}

} // namespace Good