#include "UI/UIStyle.h"

namespace Good
{

UIStyle UIStyle::Dark()
{
    UIStyle s;

    s.colors.background          = Color(0.15f, 0.15f, 0.18f, 1.0f);
    s.colors.backgroundHovered   = Color(0.20f, 0.20f, 0.24f, 1.0f);
    s.colors.backgroundActive    = Color(0.25f, 0.25f, 0.30f, 1.0f);
    s.colors.border              = Color(0.30f, 0.30f, 0.35f, 1.0f);
    s.colors.text                = Color(0.92f, 0.92f, 0.92f, 1.0f);
    s.colors.textDisabled        = Color(0.55f, 0.55f, 0.58f, 1.0f);
    s.colors.textSelected        = Color(1.00f, 1.00f, 1.00f, 1.0f);
    s.colors.button              = Color(0.26f, 0.26f, 0.32f, 1.0f);
    s.colors.buttonHovered       = Color(0.36f, 0.36f, 0.44f, 1.0f);
    s.colors.buttonActive        = Color(0.18f, 0.46f, 0.82f, 1.0f);
    s.colors.accent              = Color(0.20f, 0.50f, 0.90f, 1.0f);
    s.colors.accentHovered       = Color(0.25f, 0.60f, 1.00f, 1.0f);
    s.colors.slider              = Color(0.26f, 0.26f, 0.32f, 1.0f);
    s.colors.sliderFill          = Color(0.20f, 0.50f, 0.90f, 1.0f);
    s.colors.checkboxOff         = Color(0.22f, 0.22f, 0.26f, 1.0f);
    s.colors.checkboxOn          = Color(0.20f, 0.50f, 0.90f, 1.0f);
    s.colors.header              = Color(0.20f, 0.20f, 0.25f, 1.0f);
    s.colors.headerHovered       = Color(0.28f, 0.28f, 0.34f, 1.0f);
    s.colors.separator           = Color(0.35f, 0.35f, 0.40f, 1.0f);
    s.colors.scrollbar           = Color(0.20f, 0.20f, 0.24f, 1.0f);
    s.colors.scrollbarGrab       = Color(0.40f, 0.40f, 0.50f, 1.0f);
    s.colors.tooltip             = Color(0.10f, 0.10f, 0.12f, 0.95f);
    s.colors.windowBackground    = Color(0.13f, 0.13f, 0.16f, 1.0f);
    s.colors.titleBar            = Color(0.10f, 0.10f, 0.13f, 1.0f);
    s.colors.titleBarActive      = Color(0.15f, 0.15f, 0.20f, 1.0f);
    s.colors.inputBackground     = Color(0.10f, 0.10f, 0.13f, 1.0f);
    s.colors.inputBorder         = Color(0.30f, 0.30f, 0.38f, 1.0f);
    s.colors.inputBorderFocused  = Color(0.20f, 0.50f, 0.90f, 1.0f);
    s.colors.treeArrow           = Color(0.60f, 0.60f, 0.65f, 1.0f);
    s.colors.selection           = Color(0.20f, 0.50f, 0.90f, 0.40f);
    s.colors.progressFill        = Color(0.20f, 0.50f, 0.90f, 1.0f);
    s.colors.progressBg          = Color(0.18f, 0.18f, 0.22f, 1.0f);

    // ─── КРУПНЫЕ РАЗМЕРЫ ─────────────────────────────────────────────
    s.metrics.fontSize           = 16.0f;     // было 13
    s.metrics.itemSpacing        = 8.0f;      // было 4
    s.metrics.itemPadding        = 10.0f;     // было 6
    s.metrics.windowPadding      = 12.0f;     // было 8
    s.metrics.borderRadius       = 4.0f;
    s.metrics.scrollbarSize      = 14.0f;     // было 9
    s.metrics.checkboxSize       = 20.0f;     // было 15
    s.metrics.sliderHeight       = 8.0f;      // было 5
    s.metrics.titleBarHeight     = 32.0f;     // было 22
    s.metrics.indentWidth        = 20.0f;     // было 14
    s.metrics.borderWidth        = 1.0f;

    return s;
}

UIStyle UIStyle::Light()
{
    UIStyle s;
    s = Dark();
    s.colors.background        = Color(0.92f, 0.92f, 0.95f, 1.0f);
    s.colors.backgroundHovered = Color(0.85f, 0.85f, 0.88f, 1.0f);
    s.colors.text              = Color(0.10f, 0.10f, 0.12f, 1.0f);
    s.colors.textDisabled      = Color(0.55f, 0.55f, 0.58f, 1.0f);
    s.colors.button            = Color(0.80f, 0.80f, 0.84f, 1.0f);
    s.colors.buttonHovered     = Color(0.70f, 0.70f, 0.75f, 1.0f);
    s.colors.windowBackground  = Color(0.94f, 0.94f, 0.96f, 1.0f);
    s.colors.titleBar          = Color(0.82f, 0.82f, 0.86f, 1.0f);
    s.colors.header            = Color(0.86f, 0.86f, 0.90f, 1.0f);
    s.colors.inputBackground   = Color(1.00f, 1.00f, 1.00f, 1.0f);
    s.colors.border            = Color(0.65f, 0.65f, 0.70f, 1.0f);
    return s;
}

UIStyle UIStyle::Classic() { return Dark(); }

} // namespace Good