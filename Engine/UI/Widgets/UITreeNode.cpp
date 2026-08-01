#include "UI/Widgets/UITreeNode.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"
#include "Utilities/Hash.h"

namespace Good
{

static std::unordered_map<uint64, bool> s_OpenStates;

bool UITreeNode::Begin(const std::string& label, bool defaultOpen, bool* selected)
{
    auto& ctx     = UIContext::Get();
    auto& layout  = UILayout::Get();
    auto& renderer= UIRenderer::Get();
    auto& style   = ctx.GetStyle();

    uint64 id = ctx.GenerateID(label + "##tree");

    auto it = s_OpenStates.find(id);
    if (it == s_OpenStates.end())
    {
        s_OpenStates[id] = defaultOpen;
        it = s_OpenStates.find(id);
    }

    bool& isOpen = it->second;

    float32 h    = style.metrics.fontSize + style.metrics.itemPadding;
    Rect    rect = layout.NextItemRect(layout.GetAvailableSize().x, h);

    bool hovered = ctx.IsItemHovered(rect);
    if (hovered) ctx.SetHovered(id);

    if (hovered && ctx.GetState().mousePressed)
    {
        isOpen = !isOpen;
        if (selected) *selected = true;
    }

    bool isSelected = (selected && *selected);
    if (hovered || isSelected)
        renderer.DrawRect(rect, hovered ? style.colors.headerHovered
                                        : style.colors.selection);

    float32 arrowSize = 8.0f;
    float32 arrowX    = rect.x + 4;
    float32 arrowY    = rect.y + (h - arrowSize) * 0.5f;

    if (isOpen)
    {
        renderer.DrawTriangle(
            Vec2(arrowX,                       arrowY),
            Vec2(arrowX + arrowSize,            arrowY),
            Vec2(arrowX + arrowSize * 0.5f,    arrowY + arrowSize),
            style.colors.treeArrow);
    }
    else
    {
        renderer.DrawTriangle(
            Vec2(arrowX,                       arrowY),
            Vec2(arrowX + arrowSize,            arrowY + arrowSize * 0.5f),
            Vec2(arrowX,                       arrowY + arrowSize),
            style.colors.treeArrow);
    }

    renderer.DrawTextUI(label,
        Vec2(rect.x + arrowSize + 8,
             rect.y + (h - style.metrics.fontSize) * 0.5f),
        style.colors.text, style.metrics.fontSize);

    if (isOpen) layout.Indent(style.metrics.indentWidth);

    return isOpen;
}

void UITreeNode::End()
{
    UILayout::Get().Unindent(UIContext::Get().GetStyle().metrics.indentWidth);
}

bool UITreeNode::BeginLeaf(const std::string& label, bool* selected)
{
    auto& ctx     = UIContext::Get();
    auto& layout  = UILayout::Get();
    auto& renderer= UIRenderer::Get();
    auto& style   = ctx.GetStyle();

    float32 h    = style.metrics.fontSize + style.metrics.itemPadding;
    Rect    rect = layout.NextItemRect(layout.GetAvailableSize().x, h);

    uint64 id        = ctx.GenerateID(label + "##leaf");
    bool   hovered   = ctx.IsItemHovered(rect);
    bool   isSelected= selected && *selected;

    if (hovered) ctx.SetHovered(id);

    if (hovered && ctx.GetState().mousePressed && selected)
        *selected = true;

    if (hovered || isSelected)
        renderer.DrawRect(rect, hovered ? style.colors.headerHovered
                                        : style.colors.selection);

    renderer.DrawRect(
        Rect(rect.x + 4, rect.y + h * 0.5f - 2, 4, 4),
        style.colors.treeArrow);

    renderer.DrawTextUI(label,
        Vec2(rect.x + 16,
             rect.y + (h - style.metrics.fontSize) * 0.5f),
        style.colors.text, style.metrics.fontSize);

    return (hovered && ctx.GetState().mousePressed);
}

} // namespace Good