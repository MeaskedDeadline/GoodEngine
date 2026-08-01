#include "UI/Widgets/UITextInput.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"
#include "UI/UIInput.h"
#include "Core/Time.h"
#include "Math/MathCommon.h"

namespace Good
{

std::unordered_map<uint64, usize> UITextInput::s_CursorPositions;
float32                            UITextInput::s_BlinkTimer = 0.0f;

usize& UITextInput::GetCursor(uint64 id, usize defaultPos)
{
    auto it = s_CursorPositions.find(id);
    if (it == s_CursorPositions.end())
    {
        s_CursorPositions[id] = defaultPos;
        return s_CursorPositions[id];
    }
    return it->second;
}

bool UITextInput::DrawWithEnter(const std::string& label, std::string& value,
                                 const std::string& hint, float32 width,
                                 bool* outChanged)
{
    auto& ctx     = UIContext::Get();
    auto& layout  = UILayout::Get();
    auto& renderer= UIRenderer::Get();
    auto& style   = ctx.GetStyle();

    float32 fontSize  = style.metrics.fontSize;
    float32 charWidth = 9.0f * (fontSize / 8.0f);

    float32 h      = fontSize + style.metrics.itemPadding * 2;
    float32 labelW = label.empty() ? 0.0f
                   : static_cast<float32>(label.size()) * charWidth + 8.0f;

    Rect rect = layout.NextItemRect(labelW + width, h);
    uint64 id = ctx.GenerateID(label + "##input", &value);

    if (!label.empty())
        renderer.DrawTextUI(label,
            Vec2(rect.x, rect.y + (h - fontSize) * 0.5f),
            style.colors.text, fontSize);

    Rect inputRect(rect.x + labelW, rect.y, width, h);

    bool hovered = ctx.IsItemHovered(inputRect);
    bool focused = ctx.IsFocused(id);

    if (hovered && ctx.GetState().mousePressed)
    {
        ctx.SetFocused(id);

        float32 textStartX = inputRect.x + style.metrics.itemPadding;
        float32 clickX     = ctx.GetState().mousePos.x - textStartX;

        if (clickX <= 0)
            s_CursorPositions[id] = 0;
        else
        {
            usize pos = static_cast<usize>(clickX / charWidth + 0.5f);
            if (pos > value.size()) pos = value.size();
            s_CursorPositions[id] = pos;
        }

        s_BlinkTimer = 0.0f;
    }

    if (!hovered && ctx.GetState().mousePressed && focused)
        ctx.SetFocused(0);

    Color borderColor = focused ? style.colors.inputBorderFocused
                                : style.colors.inputBorder;
    renderer.DrawRect(inputRect, style.colors.inputBackground, style.metrics.borderRadius);
    renderer.DrawRectOutline(inputRect, borderColor, focused ? 2.0f : 1.0f);

    bool changed      = false;
    bool enterPressed = false;

    if (focused)
    {
        usize& cursor = GetCursor(id, value.size());
        if (cursor > value.size()) cursor = value.size();

        const std::string& typed = UIInput::Get().GetInputText();
        for (char c : typed)
        {
            if (c >= 32 && c < 127)
            {
                value.insert(value.begin() + cursor, c);
                ++cursor;
                changed = true;
                s_BlinkTimer = 0.0f;
            }
        }

        // Backspace
        if (UIInput::Get().IsKeyPressed(8) && cursor > 0)
        {
            value.erase(value.begin() + (cursor - 1));
            --cursor;
            changed = true;
            s_BlinkTimer = 0.0f;
        }

        // Delete
        if (UIInput::Get().IsKeyPressed(46) && cursor < value.size())
        {
            value.erase(value.begin() + cursor);
            changed = true;
            s_BlinkTimer = 0.0f;
        }

        // Left
        if (UIInput::Get().IsKeyPressed(37) && cursor > 0)
        {
            --cursor;
            s_BlinkTimer = 0.0f;
        }

        // Right
        if (UIInput::Get().IsKeyPressed(39) && cursor < value.size())
        {
            ++cursor;
            s_BlinkTimer = 0.0f;
        }

        // Home
        if (UIInput::Get().IsKeyPressed(36))
        {
            cursor = 0;
            s_BlinkTimer = 0.0f;
        }

        // End
        if (UIInput::Get().IsKeyPressed(35))
        {
            cursor = value.size();
            s_BlinkTimer = 0.0f;
        }

        // Enter
        if (UIInput::Get().IsKeyPressed(13))
            enterPressed = true;
    }

    const std::string& display  = value.empty() ? hint : value;
    Color              textColor= value.empty() ? style.colors.textDisabled
                                                : style.colors.text;

    float32 textX = inputRect.x + style.metrics.itemPadding;
    float32 textY = inputRect.y + (h - fontSize) * 0.5f;

    renderer.DrawTextUI(display, Vec2(textX, textY), textColor, fontSize);

    if (focused)
    {
        s_BlinkTimer += Time::DeltaTime();
        if (s_BlinkTimer > 1.0f) s_BlinkTimer = 0.0f;

        if (s_BlinkTimer < 0.6f)
        {
            usize cursor = GetCursor(id, value.size());
            float32 cursorX = textX + static_cast<float32>(cursor) * charWidth;

            renderer.DrawLine(
                Vec2(cursorX, inputRect.y + 4),
                Vec2(cursorX, inputRect.y + h - 4),
                Color(1.0f, 1.0f, 1.0f, 1.0f),
                2.0f);
        }
    }

    if (outChanged) *outChanged = changed;
    return enterPressed;
}

bool UITextInput::Draw(const std::string& label, std::string& value,
                       const std::string& hint, float32 width)
{
    bool changed = false;
    DrawWithEnter(label, value, hint, width, &changed);
    return changed;
}

void UITextInput::Render()
{
    if (!m_Visible) return;
}

} // namespace Good