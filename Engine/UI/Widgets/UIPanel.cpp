#include "UI/Widgets/UIPanel.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"

namespace Good
{

    bool UIPanel::Begin(const std::string& title, const Rect& rect)
    {
        auto& ctx      = UIContext::Get();
        auto& renderer = UIRenderer::Get();
        auto& style    = ctx.GetStyle();

        renderer.DrawRect(rect, style.colors.windowBackground, style.metrics.borderRadius);
        renderer.DrawRectOutline(rect, style.colors.border, style.metrics.borderWidth);

        if (!title.empty())
        {
            Rect titleBar(rect.x, rect.y, rect.width, style.metrics.titleBarHeight);
            renderer.DrawRect(titleBar, style.colors.titleBar, style.metrics.borderRadius);
            renderer.DrawTextUI(title,
                Vec2(rect.x + style.metrics.windowPadding, rect.y + 5),
                style.colors.text, style.metrics.fontSize);
        }

        float32 contentY = rect.y
                         + (title.empty() ? 0.0f : style.metrics.titleBarHeight)
                         + style.metrics.windowPadding;

        Rect contentRect(
            rect.x + style.metrics.windowPadding,
            contentY,
            rect.width  - style.metrics.windowPadding * 2,
            rect.height - style.metrics.titleBarHeight - style.metrics.windowPadding * 2);

        UILayout::Get().PushLayout(contentRect);
        return true;
    }

    void UIPanel::End()
    {
        UILayout::Get().PopLayout();
    }

    void UIPanel::BeginGroup(const std::string& /*id*/)
    {
        auto& layout = UILayout::Get();
        Vec2  cursor = layout.GetCursor();
        Vec2  avail  = layout.GetAvailableSize();
        layout.PushLayout(Rect(cursor.x, cursor.y, avail.x, avail.y));
    }

    void UIPanel::EndGroup()
    {
        UILayout::Get().PopLayout();
    }

} // namespace Good