#include "UI/Widgets/UIProgressBar.h"
#include "UI/UIRenderer.h"
#include "UI/UIContext.h"
#include "UI/UILayout.h"
#include "Math/MathCommon.h"
#include <cstring>
#include <cstdio>

namespace Good
{

    void UIProgressBar::Draw(float32 value, const Vec2& size, const std::string& overlay)
    {
        auto& ctx     = UIContext::Get();
        auto& layout  = UILayout::Get();
        auto& renderer= UIRenderer::Get();
        auto& style   = ctx.GetStyle();

        float32 w = size.x > 0 ? size.x : 200.0f;
        float32 h = size.y > 0 ? size.y : 20.0f;

        Rect rect = layout.NextItemRect(w, h);

        renderer.DrawRect(rect, style.colors.progressBg, h * 0.5f);

        float32 t = Math::Clamp(value, 0.0f, 1.0f);
        Rect fillRect(rect.x, rect.y, rect.width * t, rect.height);
        if (fillRect.width > 0)
            renderer.DrawRect(fillRect, style.colors.progressFill, h * 0.5f);

        renderer.DrawRectOutline(rect, style.colors.border, 1.0f);

        char buf[32];
        if (overlay.empty())
            std::snprintf(buf, sizeof(buf), "%.0f%%", static_cast<double>(t * 100));

        const char* displayText = overlay.empty() ? buf : overlay.c_str();

        float32 textW = static_cast<float32>(strlen(displayText)) * 8.0f;

        renderer.DrawTextUI(displayText,
            Vec2(rect.x + (rect.width  - textW) * 0.5f,
                 rect.y + (rect.height - style.metrics.fontSize) * 0.5f),
            style.colors.text, style.metrics.fontSize);
    }

    void UIProgressBar::Render()
    {
        if (!m_Visible) return;
    }

} // namespace Good