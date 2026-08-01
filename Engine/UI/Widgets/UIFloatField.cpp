#include "UI/Widgets/UIFloatField.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"
#include "UI/UIInput.h"
#include "Core/Time.h"
#include "Math/MathCommon.h"
#include <cstdio>
#include <cstdlib>

namespace Good
{

std::unordered_map<uint64, bool>        UIFloatField::s_EditingState;
std::unordered_map<uint64, std::string> UIFloatField::s_EditingText;
std::unordered_map<uint64, float32>     UIFloatField::s_DragStartValue;
std::unordered_map<uint64, float32>     UIFloatField::s_DragStartMouseX;
std::unordered_map<uint64, float64>     UIFloatField::s_LastClickTime;

bool UIFloatField::Draw(const std::string& label, float32& value,
                         float32 speed, float32 width)
{
    auto& ctx      = UIContext::Get();
    auto& layout   = UILayout::Get();
    auto& renderer = UIRenderer::Get();
    auto& style    = ctx.GetStyle();

    float32 fontSize  = style.metrics.fontSize;
    float32 charWidth = 9.0f * (fontSize / 8.0f);

    float32 h      = fontSize + style.metrics.itemPadding * 2;
    float32 labelW = label.empty() ? 0.0f
                   : static_cast<float32>(label.size()) * charWidth + 4.0f;
    float32 totalW = labelW + width;

    Rect rect = layout.NextItemRect(totalW, h);
    uint64 id = ctx.GenerateID(label + "##float", &value);

    // Цвет label по оси
    Color labelColor = style.colors.text;
    if (label == "X") labelColor = Color(0.9f, 0.3f, 0.3f, 1.0f);
    if (label == "Y") labelColor = Color(0.3f, 0.9f, 0.3f, 1.0f);
    if (label == "Z") labelColor = Color(0.3f, 0.5f, 0.9f, 1.0f);

    if (!label.empty())
    {
        renderer.DrawTextUI(label,
            Vec2(rect.x, rect.y + (h - fontSize) * 0.5f),
            labelColor, fontSize);
    }

    Rect fieldRect(rect.x + labelW, rect.y, width, h);
    bool hovered  = ctx.IsItemHovered(fieldRect);
    bool& editing = s_EditingState[id];
    bool changed  = false;

    // Фон
    Color bgColor = editing ? Color(0.06f, 0.06f, 0.10f, 1.0f)
                   : hovered ? Color(0.14f, 0.14f, 0.18f, 1.0f)
                   :            Color(0.11f, 0.11f, 0.14f, 1.0f);

    // Цветная полоска слева по оси
    Color accentColor = style.colors.accent;
    if (label == "X") accentColor = Color(0.9f, 0.3f, 0.3f, 1.0f);
    if (label == "Y") accentColor = Color(0.3f, 0.9f, 0.3f, 1.0f);
    if (label == "Z") accentColor = Color(0.3f, 0.5f, 0.9f, 1.0f);

    Color borderColor = editing ? style.colors.inputBorderFocused
                                : style.colors.inputBorder;

    renderer.DrawRect(fieldRect, bgColor, style.metrics.borderRadius);
    renderer.DrawRectOutline(fieldRect, borderColor, editing ? 2.0f : 1.0f);

    // Цветная полоска слева
    renderer.DrawRect(Rect(fieldRect.x, fieldRect.y + 2,
                           3.0f, fieldRect.height - 4), accentColor);

    if (editing)
    {
        // ═══ Текстовый ввод ═══
        std::string& text = s_EditingText[id];

        const std::string& typed = UIInput::Get().GetInputText();
        for (char c : typed)
        {
            if ((c >= '0' && c <= '9') || c == '.' || c == '-')
                text += c;
        }

        if (UIInput::Get().IsKeyPressed(8) && !text.empty())
            text.pop_back();

        // Enter — применить
        if (UIInput::Get().IsKeyPressed(13))
        {
            char* end = nullptr;
            float32 parsed = std::strtof(text.c_str(), &end);
            if (end != text.c_str())
            {
                value = parsed;
                changed = true;
            }
            editing = false;
            ctx.SetFocused(0);
        }

        // Escape — отменить
        if (UIInput::Get().IsKeyPressed(27))
        {
            editing = false;
            ctx.SetFocused(0);
        }

        // Клик мимо — применить
        if (!hovered && ctx.GetState().mousePressed)
        {
            char* end = nullptr;
            float32 parsed = std::strtof(text.c_str(), &end);
            if (end != text.c_str())
            {
                value = parsed;
                changed = true;
            }
            editing = false;
            ctx.SetFocused(0);
        }

        // Рисуем текст
        renderer.DrawTextUI(text,
            Vec2(fieldRect.x + 6, fieldRect.y + (h - fontSize) * 0.5f),
            style.colors.text, fontSize);

        // Курсор
        static float32 blink = 0.0f;
        blink += Time::DeltaTime();
        if (blink > 1.0f) blink = 0.0f;
        if (blink < 0.6f)
        {
            float32 cursorX = fieldRect.x + 6 +
                static_cast<float32>(text.size()) * charWidth;
            renderer.DrawLine(
                Vec2(cursorX, fieldRect.y + 4),
                Vec2(cursorX, fieldRect.y + h - 4),
                style.colors.text, 1.5f);
        }
    }
    else
    {
        // ═══ Drag / Двойной клик ═══

        if (hovered && ctx.GetState().mousePressed)
        {
            float64 now = Time::Elapsed();
            float64 lastClick = s_LastClickTime[id];
            s_LastClickTime[id] = now;

            // Двойной клик — менее 0.4 секунды между кликами
            if (now - lastClick < 0.4)
            {
                editing = true;
                char buf[32];
                std::snprintf(buf, sizeof(buf), "%.3f", static_cast<double>(value));
                s_EditingText[id] = buf;
                ctx.SetFocused(id);
            }
            else
            {
                // Одиночный клик — начинаем drag
                s_DragStartValue[id]  = value;
                s_DragStartMouseX[id] = ctx.GetState().mousePos.x;
                ctx.SetActive(id);
            }
        }

        // Drag
        if (ctx.IsActive(id) && ctx.GetState().mouseDown)
        {
            float32 dx = ctx.GetState().mousePos.x - s_DragStartMouseX[id];
            float32 newValue = s_DragStartValue[id] + dx * speed;
            if (Math::Abs(newValue - value) > 0.0001f)
            {
                value = newValue;
                changed = true;
            }
        }

        if (ctx.IsActive(id) && !ctx.GetState().mouseDown)
            ctx.SetActive(0);

        // Отображаем значение
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%.2f", static_cast<double>(value));

        renderer.DrawTextUI(buf,
            Vec2(fieldRect.x + 6, fieldRect.y + (h - fontSize) * 0.5f),
            hovered ? style.colors.textSelected : style.colors.text,
            fontSize);

        // Подсказка drag
        if (hovered && !ctx.IsActive(id))
        {
            renderer.DrawTextUI("<->",
                Vec2(fieldRect.x + width - 30,
                     fieldRect.y + (h - fontSize) * 0.5f),
                Color(0.4f, 0.4f, 0.5f, 0.6f), fontSize - 2);
        }
    }

    return changed;
}

void UIFloatField::Render()
{
    if (!m_Visible) return;
}

} // namespace Good