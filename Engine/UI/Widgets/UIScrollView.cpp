#include "UI/Widgets/UIScrollView.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"
#include "UI/UIInput.h"
#include "Math/MathCommon.h"
#include "Utilities/Hash.h"

namespace Good
{

    bool UIScrollView::Begin(const std::string& id, const Rect& rect,
                             float32& scrollY, float32 contentH)
    {
        auto& ctx     = UIContext::Get();
        auto& renderer= UIRenderer::Get();
        auto& style   = ctx.GetStyle();

        renderer.DrawRect(rect, style.colors.background);
        renderer.DrawRectOutline(rect, style.colors.border, 1.0f);

        float32 viewH      = rect.height;
        float32 scrollbarW = style.metrics.scrollbarSize;

        if (ctx.IsItemHovered(rect))
        {
            float32 scroll = UIInput::Get().GetScrollY();
            scrollY = Math::Clamp(scrollY - scroll * 20.0f, 0.0f,
                                  Math::Max(0.0f, contentH - viewH));
        }

        if (contentH > viewH)
        {
            float32 trackH     = viewH;
            float32 thumbRatio = Math::Clamp(viewH / contentH, 0.05f, 1.0f);
            float32 thumbH     = trackH * thumbRatio;
            float32 scrollRatio= (contentH - viewH > 0) ? scrollY / (contentH - viewH) : 0;
            float32 thumbY     = rect.y + scrollRatio * (trackH - thumbH);

            Rect trackRect(rect.x + rect.width - scrollbarW, rect.y, scrollbarW, trackH);
            Rect thumbRect(trackRect.x + 2, thumbY, scrollbarW - 4, thumbH);

            renderer.DrawRect(trackRect, style.colors.scrollbar);
            renderer.DrawRect(thumbRect, style.colors.scrollbarGrab, 4.0f);
        }

        Rect contentRect(rect.x, rect.y - scrollY,
                         rect.width - scrollbarW, contentH);

        renderer.PushClipRect(rect);
        UILayout::Get().PushLayout(contentRect);
        return true;
    }

    void UIScrollView::End()
    {
        UILayout::Get().PopLayout();
        UIRenderer::Get().PopClipRect();
    }

} // namespace Good