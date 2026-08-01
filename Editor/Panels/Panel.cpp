#include "Panel.h"
#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UIInput.h"
#include "Input/Input.h"
#include "Input/MouseCodes.h"
#include "Platform/Cursor.h"
#include "Math/MathCommon.h"

namespace Good
{

void Panel::HandleDragResize()
{
    if (!m_Visible) return;

    auto& ctx   = UIContext::Get();
    auto& input = Input::Get();

    float32 mx = input.GetMouseX();
    float32 my = input.GetMouseY();

    // Title bar rect
    Rect titleBar(m_Rect.x, m_Rect.y, m_Rect.width, TITLE_BAR_H);

    // Resize edges
    Rect rightEdge(m_Rect.x + m_Rect.width - RESIZE_EDGE, m_Rect.y,
                   RESIZE_EDGE, m_Rect.height);
    Rect bottomEdge(m_Rect.x, m_Rect.y + m_Rect.height - RESIZE_EDGE,
                    m_Rect.width, RESIZE_EDGE);
    Rect cornerRect(m_Rect.x + m_Rect.width - RESIZE_EDGE * 2,
                    m_Rect.y + m_Rect.height - RESIZE_EDGE * 2,
                    RESIZE_EDGE * 2, RESIZE_EDGE * 2);

    bool overTitle  = ctx.IsItemHovered(titleBar);
    bool overRight  = ctx.IsItemHovered(rightEdge);
    bool overBottom = ctx.IsItemHovered(bottomEdge);
    bool overCorner = ctx.IsItemHovered(cornerRect);

    // Курсор
    if (!m_Dragging && !m_Resizing)
    {
        if (overCorner)       Cursor::SetType(CursorType::ResizeAll);
        else if (overRight)   Cursor::SetType(CursorType::ResizeH);
        else if (overBottom)  Cursor::SetType(CursorType::ResizeV);
        else if (overTitle)   Cursor::SetType(CursorType::Arrow);
    }

    // ═══ Начало drag ═══
    if (overTitle && !overCorner && !overRight && !overBottom &&
        input.IsMouseButtonPressed(MouseButton::Left) &&
        !m_Dragging && !m_Resizing)
    {
        m_Dragging    = true;
        m_DragOffsetX = mx - m_Rect.x;
        m_DragOffsetY = my - m_Rect.y;
    }

    // ═══ Начало resize ═══
    if (!m_Dragging && input.IsMouseButtonPressed(MouseButton::Left) && !m_Resizing)
    {
        if (overCorner)
        {
            m_Resizing    = true;
            m_ResizeEdge  = 3;
            m_ResizeStartW = m_Rect.width;
            m_ResizeStartH = m_Rect.height;
            m_ResizeStartX = mx;
            m_ResizeStartY = my;
        }
        else if (overRight)
        {
            m_Resizing    = true;
            m_ResizeEdge  = 1;
            m_ResizeStartW = m_Rect.width;
            m_ResizeStartX = mx;
        }
        else if (overBottom)
        {
            m_Resizing    = true;
            m_ResizeEdge  = 2;
            m_ResizeStartH = m_Rect.height;
            m_ResizeStartY = my;
        }
    }

    // ═══ Продолжение drag ═══
    if (m_Dragging)
    {
        if (input.IsMouseButtonDown(MouseButton::Left))
        {
            m_Rect.x = mx - m_DragOffsetX;
            m_Rect.y = my - m_DragOffsetY;

            // Clamp в пределах экрана
            if (m_Rect.x < 0) m_Rect.x = 0;
            if (m_Rect.y < 0) m_Rect.y = 0;

            float32 maxX = static_cast<float32>(ctx.GetViewportW()) - 50;
            float32 maxY = static_cast<float32>(ctx.GetViewportH()) - TITLE_BAR_H;
            if (m_Rect.x > maxX) m_Rect.x = maxX;
            if (m_Rect.y > maxY) m_Rect.y = maxY;
        }
        else
        {
            m_Dragging = false;
        }
    }

    // ═══ Продолжение resize ═══
    if (m_Resizing)
    {
        if (input.IsMouseButtonDown(MouseButton::Left))
        {
            float32 dx = mx - m_ResizeStartX;
            float32 dy = my - m_ResizeStartY;

            if (m_ResizeEdge == 1 || m_ResizeEdge == 3)
            {
                m_Rect.width = Math::Max(MIN_WIDTH, m_ResizeStartW + dx);
            }
            if (m_ResizeEdge == 2 || m_ResizeEdge == 3)
            {
                m_Rect.height = Math::Max(MIN_HEIGHT, m_ResizeStartH + dy);
            }
        }
        else
        {
            m_Resizing = false;
            Cursor::SetType(CursorType::Arrow);
        }
    }
}

void Panel::DrawPanelFrame()
{
    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();

    // Фон панели
    renderer.DrawRect(m_Rect, style.colors.windowBackground);
    renderer.DrawRectOutline(m_Rect, style.colors.border, 1.0f);

    // Title bar
    Rect titleBar(m_Rect.x, m_Rect.y, m_Rect.width, TITLE_BAR_H);
    Color titleColor = m_Focused
        ? style.colors.titleBarActive
        : style.colors.titleBar;
    renderer.DrawRect(titleBar, titleColor);

    // Линия под title bar
    renderer.DrawLine(
        Vec2(m_Rect.x, m_Rect.y + TITLE_BAR_H),
        Vec2(m_Rect.x + m_Rect.width, m_Rect.y + TITLE_BAR_H),
        style.colors.border, 1.0f);

    // Название панели
    renderer.DrawTextUI(GetTitle(),
        Vec2(m_Rect.x + 8, m_Rect.y + 6),
        style.colors.text, 13.0f);

    // Resize индикатор в правом нижнем углу
    float32 rx = m_Rect.x + m_Rect.width - 12;
    float32 ry = m_Rect.y + m_Rect.height - 12;
    Color resizeColor(0.4f, 0.4f, 0.5f, 0.5f);
    renderer.DrawLine(Vec2(rx + 2, ry + 10), Vec2(rx + 10, ry + 2), resizeColor, 1.0f);
    renderer.DrawLine(Vec2(rx + 5, ry + 10), Vec2(rx + 10, ry + 5), resizeColor, 1.0f);
    renderer.DrawLine(Vec2(rx + 8, ry + 10), Vec2(rx + 10, ry + 8), resizeColor, 1.0f);
}

} // namespace Good