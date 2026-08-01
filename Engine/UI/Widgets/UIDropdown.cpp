#include "UI/Widgets/UIDropdown.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"
#include "Utilities/Hash.h"

namespace Good
{

std::unordered_map<uint64, bool> UIDropdown::s_OpenState;

bool UIDropdown::Draw(const std::string& label,
                      const std::vector<std::string>& items,
                      int32& selectedIndex,
                      float32 width)
{
    auto& ctx     = UIContext::Get();
    auto& layout  = UILayout::Get();
    auto& renderer= UIRenderer::Get();
    auto& style   = ctx.GetStyle();

    float32 h      = style.metrics.fontSize + style.metrics.itemPadding * 2;
    float32 labelW = label.empty() ? 0.0f
                   : static_cast<float32>(label.size()) * 8.0f + 8.0f;

    Rect rect = layout.NextItemRect(labelW + width, h);
    uint64 id = ctx.GenerateID(label + "##dropdown");

    if (!label.empty())
        renderer.DrawTextUI(label,
            Vec2(rect.x, rect.y + (h - style.metrics.fontSize) * 0.5f),
            style.colors.text, style.metrics.fontSize);

    Rect boxRect(rect.x + labelW, rect.y, width, h);
    bool hovered  = ctx.IsItemHovered(boxRect);
    bool& isOpen  = s_OpenState[id];

    if (hovered) ctx.SetHovered(id);
    if (hovered && ctx.GetState().mousePressed) isOpen = !isOpen;

    Color bg = hovered ? style.colors.backgroundHovered : style.colors.background;
    renderer.DrawRect(boxRect, bg, style.metrics.borderRadius);
    renderer.DrawRectOutline(boxRect, style.colors.border, 1.0f);

    std::string current = (selectedIndex >= 0 &&
                           selectedIndex < static_cast<int32>(items.size()))
                        ? items[selectedIndex] : "None";

    renderer.DrawTextUI(current,
        Vec2(boxRect.x + style.metrics.itemPadding,
             boxRect.y + (h - style.metrics.fontSize) * 0.5f),
        style.colors.text, style.metrics.fontSize);

    renderer.DrawTriangle(
        Vec2(boxRect.x + boxRect.width - 18, boxRect.y + h * 0.35f),
        Vec2(boxRect.x + boxRect.width - 8,  boxRect.y + h * 0.35f),
        Vec2(boxRect.x + boxRect.width - 13, boxRect.y + h * 0.65f),
        style.colors.textDisabled);

    bool changed = false;
    if (isOpen)
    {
        float32 itemH = h;
        Rect dropRect(boxRect.x, boxRect.y + h,
                      width, static_cast<float32>(items.size()) * itemH);

        renderer.DrawRect(dropRect, style.colors.windowBackground);
        renderer.DrawRectOutline(dropRect, style.colors.border, 1.0f);

        for (int32 i = 0; i < static_cast<int32>(items.size()); ++i)
        {
            Rect itemRect(dropRect.x, dropRect.y + i * itemH, width, itemH);
            bool itemHovered = ctx.IsItemHovered(itemRect);

            if (itemHovered)
                renderer.DrawRect(itemRect, style.colors.selection);

            renderer.DrawTextUI(items[i],
                Vec2(itemRect.x + style.metrics.itemPadding,
                     itemRect.y + (itemH - style.metrics.fontSize) * 0.5f),
                itemHovered ? style.colors.textSelected : style.colors.text,
                style.metrics.fontSize);

            if (itemHovered && ctx.GetState().mousePressed)
            {
                selectedIndex = i;
                isOpen        = false;
                changed       = true;
            }
        }
    }

    return changed;
}

void UIDropdown::Render()
{
    if (!m_Visible) return;
}

} // namespace Good