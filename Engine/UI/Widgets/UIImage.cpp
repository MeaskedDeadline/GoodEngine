#include "UIImage.h"
#include "../UIRenderer.h"
#include "../UIContext.h"
#include "../UILayout.h"

namespace Good
{

    void UIImage::Draw(uint32 texID, const Rect& rect, const Color& tint)
    {
        UIRenderer::Get().DrawImage(texID, rect, tint);
    }

    void UIImage::DrawRounded(uint32 texID, const Rect& rect, float32 /*radius*/, const Color& tint)
    {
        UIRenderer::Get().DrawImage(texID, rect, tint);
    }

    void UIImage::Render()
    {
        if (!m_Visible) return;
        Draw(m_TextureID, m_Rect);
    }

} // namespace Good