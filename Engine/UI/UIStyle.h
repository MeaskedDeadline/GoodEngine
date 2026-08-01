#pragma once
#include "../Core/Core.h"
#include "../Math/Vec2.h"
#include "../Math/Vec4.h"
#include "../Math/Color.h"

namespace Good
{

struct UIColors
{
    Color background{0.15f, 0.15f, 0.18f, 1.0f};
    Color backgroundHovered{0.20f, 0.20f, 0.24f, 1.0f};
    Color backgroundActive{0.25f, 0.25f, 0.30f, 1.0f};
    Color border{0.30f, 0.30f, 0.35f, 1.0f};
    Color text{0.92f, 0.92f, 0.92f, 1.0f};
    Color textDisabled{0.50f, 0.50f, 0.50f, 1.0f};
    Color textSelected{1.00f, 1.00f, 1.00f, 1.0f};
    Color button{0.26f, 0.26f, 0.32f, 1.0f};
    Color buttonHovered{0.36f, 0.36f, 0.44f, 1.0f};
    Color buttonActive{0.18f, 0.46f, 0.82f, 1.0f};
    Color accent{0.20f, 0.50f, 0.90f, 1.0f};
    Color accentHovered{0.25f, 0.60f, 1.00f, 1.0f};
    Color slider{0.26f, 0.26f, 0.32f, 1.0f};
    Color sliderFill{0.20f, 0.50f, 0.90f, 1.0f};
    Color checkboxOff{0.22f, 0.22f, 0.26f, 1.0f};
    Color checkboxOn{0.20f, 0.50f, 0.90f, 1.0f};
    Color header{0.20f, 0.20f, 0.25f, 1.0f};
    Color headerHovered{0.28f, 0.28f, 0.34f, 1.0f};
    Color separator{0.35f, 0.35f, 0.40f, 1.0f};
    Color scrollbar{0.20f, 0.20f, 0.24f, 1.0f};
    Color scrollbarGrab{0.40f, 0.40f, 0.50f, 1.0f};
    Color tooltip{0.10f, 0.10f, 0.12f, 0.95f};
    Color windowBackground{0.13f, 0.13f, 0.16f, 0.98f};
    Color titleBar{0.10f, 0.10f, 0.13f, 1.0f};
    Color titleBarActive{0.15f, 0.15f, 0.20f, 1.0f};
    Color inputBackground{0.10f, 0.10f, 0.13f, 1.0f};
    Color inputBorder{0.30f, 0.30f, 0.38f, 1.0f};
    Color inputBorderFocused{0.20f, 0.50f, 0.90f, 1.0f};
    Color treeArrow{0.60f, 0.60f, 0.65f, 1.0f};
    Color selection{0.20f, 0.50f, 0.90f, 0.40f};
    Color progressFill{0.20f, 0.50f, 0.90f, 1.0f};
    Color progressBg{0.18f, 0.18f, 0.22f, 1.0f};
};

struct UIMetrics
{
    float32 fontSize{14.0f};
    float32 itemSpacing{4.0f};
    float32 itemPadding{8.0f};
    float32 windowPadding{10.0f};
    float32 borderRadius{4.0f};
    float32 scrollbarSize{10.0f};
    float32 checkboxSize{16.0f};
    float32 sliderHeight{6.0f};
    float32 titleBarHeight{24.0f};
    float32 indentWidth{16.0f};
    float32 borderWidth{1.0f};
};

struct UIStyle
{
    UIColors  colors;
    UIMetrics metrics;

    static UIStyle Dark();
    static UIStyle Light();
    static UIStyle Classic();
};

} // namespace Good