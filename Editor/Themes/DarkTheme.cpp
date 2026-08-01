#include "Themes/DarkTheme.h"

namespace Good
{

UIStyle DarkTheme::GetStyle() const
{
    UIStyle s;

    s.colors.background          = Color(0.13f, 0.13f, 0.16f, 1.0f);
    s.colors.backgroundHovered   = Color(0.18f, 0.18f, 0.22f, 1.0f);
    s.colors.backgroundActive    = Color(0.22f, 0.22f, 0.28f, 1.0f);
    s.colors.border              = Color(0.28f, 0.28f, 0.34f, 1.0f);
    s.colors.text                = Color(0.92f, 0.92f, 0.92f, 1.0f);
    s.colors.textDisabled        = Color(0.55f, 0.55f, 0.58f, 1.0f);
    s.colors.textSelected        = Color(1.00f, 1.00f, 1.00f, 1.0f);
    s.colors.button              = Color(0.24f, 0.24f, 0.30f, 1.0f);
    s.colors.buttonHovered       = Color(0.34f, 0.34f, 0.42f, 1.0f);
    s.colors.buttonActive        = Color(0.16f, 0.44f, 0.80f, 1.0f);
    s.colors.accent              = Color(0.18f, 0.48f, 0.88f, 1.0f);
    s.colors.accentHovered       = Color(0.22f, 0.58f, 0.98f, 1.0f);
    s.colors.header              = Color(0.18f, 0.18f, 0.24f, 1.0f);
    s.colors.headerHovered       = Color(0.26f, 0.26f, 0.32f, 1.0f);
    s.colors.windowBackground    = Color(0.12f, 0.12f, 0.15f, 1.0f);
    s.colors.titleBar            = Color(0.09f, 0.09f, 0.12f, 1.0f);
    s.colors.titleBarActive      = Color(0.13f, 0.13f, 0.18f, 1.0f);
    s.colors.inputBackground     = Color(0.08f, 0.08f, 0.10f, 1.0f);
    s.colors.inputBorder         = Color(0.28f, 0.28f, 0.36f, 1.0f);
    s.colors.inputBorderFocused  = Color(0.18f, 0.48f, 0.88f, 1.0f);
    s.colors.separator           = Color(0.28f, 0.28f, 0.34f, 1.0f);
    s.colors.scrollbar           = Color(0.16f, 0.16f, 0.20f, 1.0f);
    s.colors.scrollbarGrab       = Color(0.38f, 0.38f, 0.48f, 1.0f);
    s.colors.treeArrow           = Color(0.60f, 0.60f, 0.65f, 1.0f);
    s.colors.selection           = Color(0.18f, 0.48f, 0.88f, 0.35f);
    s.colors.progressFill        = Color(0.18f, 0.48f, 0.88f, 1.0f);
    s.colors.progressBg          = Color(0.14f, 0.14f, 0.18f, 1.0f);

    // ─── КРУПНЫЕ РАЗМЕРЫ ─────────────────────────────────────────────
    s.metrics.fontSize           = 16.0f;
    s.metrics.itemSpacing        = 8.0f;
    s.metrics.itemPadding        = 10.0f;
    s.metrics.windowPadding      = 12.0f;
    s.metrics.borderRadius       = 4.0f;
    s.metrics.scrollbarSize      = 14.0f;
    s.metrics.checkboxSize       = 20.0f;
    s.metrics.sliderHeight       = 8.0f;
    s.metrics.titleBarHeight     = 32.0f;
    s.metrics.indentWidth        = 20.0f;
    s.metrics.borderWidth        = 1.0f;

    return s;
}

} // namespace Good