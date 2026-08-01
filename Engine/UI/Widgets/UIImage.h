#pragma once
#include "UIWidget.h"

namespace Good
{

    class UIImage final : public UIWidget
    {
    public:
        static void Draw(uint32 textureID, const Rect& rect, const Color& tint = Color::White());
        static void DrawRounded(uint32 textureID, const Rect& rect, float32 radius, const Color& tint = Color::White());
        void Render() override;

    private:
        uint32 m_TextureID{0};
    };

} // namespace Good