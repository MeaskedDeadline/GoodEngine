#include "UI/Widgets/UIWindow.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"
#include "Utilities/Hash.h"

namespace Good
{

static std::unordered_map<uint64, Vec2> s_DragOffsets;

bool UIWindow::Begin(const std::string& title, bool& open,
                     Rect& rect, UIWindowFlags flags)
{
    if (!open) return false;

    auto& ctx     = UIContext::Get();
    auto& renderer= UIRenderer::Get();
    auto& style   = ctx.GetStyle();

    uint64 id = ctx.GenerateID(title + "##win");

    bool hasTitle = !(static_cast<uint32>(flags) &
                      static_cast<uint32>(UIWindowFlags::NoTitleBar));
    bool canClose = !(static_cast<uint32>(flags) &
                      static_cast<uint32>(UIWindowFlags::NoClose));
    bool canMove  = !(static_cast<uint32>(flags) &
                      static_cast<uint32>(UIWindowFlags::NoMove));

    renderer.DrawRect(rect, style.colors.windowBackground, style.metrics.borderRadius);
    renderer.DrawRectOutline(rect, style.colors.border, 1.0f);

    float32 titleH = style.metrics.titleBarHeight;

    if (hasTitle)
    {
        Rect titleBar(rect.x, rect.y, rect.width, titleH);
        bool titleHovered = ctx.IsItemHovered(titleBar);

        renderer.DrawRect(titleBar,
            titleHovered ? style.colors.titleBarActive : style.colors.titleBar,
            style.metrics.borderRadius);

        renderer.DrawTextUI(title,
            Vec2(rect.x + style.metrics.windowPadding,
                 rect.y + (titleH - style.metrics.fontSize) * 0.5f),
            style.colors.text, style.metrics.fontSize);

        if (canMove && titleHovered && ctx.GetState().mousePressed)
        {
            s_DragOffsets[id] = Vec2(
                ctx.GetState().mousePos.x - rect.x,
                ctx.GetState().mousePos.y - rect.y);
            ctx.SetActive(id);
        }

        if (ctx.IsActive(id) && ctx.GetState().mouseDown)
        {
            rect.x = ctx.GetState().mousePos.x - s_DragOffsets[id].x;
            rect.y = ctx.GetState().mousePos.y - s_DragOffsets[id].y;
        }

        if (canClose)
        {
            Rect closeBtn(rect.x + rect.width - titleH, rect.y, titleH, titleH);
            bool closeHovered = ctx.IsItemHovered(closeBtn);

            renderer.DrawRect(closeBtn,
                closeHovered ? Color(0.8f, 0.2f, 0.2f, 1.0f)
                             : style.colors.buttonHovered,
                style.metrics.borderRadius);

            renderer.DrawTextUI("X",
                Vec2(closeBtn.x + (titleH - 8) * 0.5f,
                     closeBtn.y + (titleH - 14) * 0.5f),
                style.colors.text, style.metrics.fontSize);

            if (closeHovered && ctx.GetState().mouseReleased)
                open = false;
        }
    }

    float32 contentY = rect.y
                     + (hasTitle ? titleH : 0.0f)
                     + style.metrics.windowPadding;

    Rect contentRect(
        rect.x + style.metrics.windowPadding,
        contentY,
        rect.width  - style.metrics.windowPadding * 2,
        rect.height - (hasTitle ? titleH : 0.0f) - style.metrics.windowPadding * 2);

    UILayout::Get().PushLayout(contentRect);
    return open;
}

void UIWindow::End()
{
    UILayout::Get().PopLayout();
}

} // namespace Good