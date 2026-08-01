#include "UI/Widgets/UISlider.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"
#include "Math/MathCommon.h"
#include <cstdio>

namespace Good
{

bool UISlider::DrawFloat(const std::string& label, float32& value, float32 mn, float32 mx)
{
    auto& ctx     = UIContext::Get();
    auto& layout  = UILayout::Get();
    auto& renderer= UIRenderer::Get();
    auto& style   = ctx.GetStyle();

    float32 h      = style.metrics.fontSize + style.metrics.itemPadding * 2;
    float32 labelW = static_cast<float32>(label.size()) * 10.0f + 8.0f;
    float32 sliderW= 160.0f;
    float32 totalW = labelW + sliderW + 60.0f;

    Rect rect = layout.NextItemRect(totalW, h);

    // ID уникален благодаря адресу значения (у каждого слайдера свой)
    uint64 id = ctx.GenerateID(label, &value);

    renderer.DrawTextUI(label,
        Vec2(rect.x, rect.y + (h - style.metrics.fontSize) * 0.5f),
        style.colors.text, style.metrics.fontSize);

    Rect trackRect(rect.x + labelW,
                   rect.y + (h - style.metrics.sliderHeight) * 0.5f,
                   sliderW, style.metrics.sliderHeight);

    // Расширяем hitbox для удобства клика
    Rect hitRect(trackRect.x, rect.y, sliderW, h);

    bool hovered = ctx.IsItemHovered(hitRect);
    bool active  = ctx.IsActive(id);

    if (hovered) ctx.SetHovered(id);

    if (hovered && ctx.GetState().mousePressed) ctx.SetActive(id);

    bool changed = false;
    if (ctx.IsActive(id) && ctx.GetState().mouseDown)
    {
        float32 t = (ctx.GetState().mousePos.x - trackRect.x) / trackRect.width;
        t         = Math::Clamp(t, 0.0f, 1.0f);
        float32 newV = Math::Lerp(mn, mx, t);
        if (Math::Abs(newV - value) > 0.0001f) { value = newV; changed = true; }
    }

    renderer.DrawRect(trackRect, style.colors.slider, style.metrics.sliderHeight * 0.5f);

    float32 t = (mx > mn) ? Math::Clamp((value - mn) / (mx - mn), 0.0f, 1.0f) : 0.0f;
    Rect fillRect(trackRect.x, trackRect.y, trackRect.width * t, trackRect.height);
    if (fillRect.width > 0)
        renderer.DrawRect(fillRect, style.colors.sliderFill, style.metrics.sliderHeight * 0.5f);

    float32 thumbX = trackRect.x + trackRect.width * t - 6.0f;
    float32 thumbH = style.metrics.sliderHeight + 8.0f;
    renderer.DrawRect(
        Rect(thumbX, trackRect.y - 4.0f, 12.0f, thumbH),
        (hovered || active) ? style.colors.accentHovered : style.colors.accent,
        6.0f);

    char buf[32];
    std::snprintf(buf, sizeof(buf), "%.2f", static_cast<double>(value));
    renderer.DrawTextUI(buf,
        Vec2(trackRect.x + trackRect.width + 8,
             rect.y + (h - style.metrics.fontSize) * 0.5f),
        style.colors.textDisabled, style.metrics.fontSize);

    return changed;
}

bool UISlider::DrawInt(const std::string& label, int32& value, int32 mn, int32 mx)
{
    float32 fVal    = static_cast<float32>(value);
    bool    changed = DrawFloat(label, fVal,
                                static_cast<float32>(mn),
                                static_cast<float32>(mx));
    if (changed) value = static_cast<int32>(fVal);
    return changed;
}

void UISlider::Render()
{
    if (!m_Visible) return;
}

} // namespace Good