#include "UILayout.h"

namespace Good
{

UILayout& UILayout::Get()
{
    static UILayout s_Instance;
    return s_Instance;
}

void UILayout::PushLayout(const Rect& bounds, UILayoutDirection dir)
{
    UILayoutState state;
    state.cursor    = Vec2(bounds.x, bounds.y);
    state.size      = Vec2(bounds.width, bounds.height);
    state.direction = dir;
    state.sameLine  = false;
    m_Stack.push_back(state);
}

void UILayout::PopLayout()
{
    if (!m_Stack.empty()) m_Stack.pop_back();
}

Rect UILayout::NextItemRect(float32 w, float32 h)
{
    if (m_Stack.empty()) return Rect(0,0,w,h);

    UILayoutState& state = m_Stack.back();

    float32 useW = (m_NextItemWidth > 0) ? m_NextItemWidth : w;
    m_NextItemWidth = -1.0f;

    Rect rect(state.cursor.x + m_IndentLevel,
              state.cursor.y,
              useW, h);

    if (state.sameLine)
    {
        state.cursor.x += useW + state.itemSpacing;
        state.sameLine  = false;
    }
    else
    {
        if (state.direction == UILayoutDirection::Vertical)
        {
            state.cursor.y += h + state.itemSpacing;
        }
        else
        {
            state.cursor.x += useW + state.itemSpacing;
        }
    }

    state.rowHeight = Math::Max(state.rowHeight, h);
    return rect;
}

Vec2 UILayout::GetCursor() const
{
    return m_Stack.empty() ? Vec2(0,0) : m_Stack.back().cursor;
}

Vec2 UILayout::GetAvailableSize() const
{
    if (m_Stack.empty()) return Vec2(0,0);
    const UILayoutState& s = m_Stack.back();
    return Vec2(
        s.size.x - (m_Stack.back().cursor.x - m_Stack.back().size.x),
        s.size.y - (m_Stack.back().cursor.y - m_Stack.back().cursor.y));
}

void UILayout::SameLine(float32 /*spacing*/)
{
    if (!m_Stack.empty()) m_Stack.back().sameLine = true;
}

void UILayout::NewLine()
{
    if (!m_Stack.empty())
    {
        UILayoutState& s = m_Stack.back();
        s.cursor.y += s.rowHeight + s.itemSpacing;
        s.cursor.x  = m_Stack.back().size.x;
        s.rowHeight = 0;
    }
}

void UILayout::Indent(float32 amount)   { m_IndentLevel += amount; }
void UILayout::Unindent(float32 amount) { m_IndentLevel = Math::Max(0.0f, m_IndentLevel - amount); }
void UILayout::SetNextItemWidth(float32 w) { m_NextItemWidth = w; }

void UILayout::Spacing(float32 amount)
{
    if (!m_Stack.empty())
        m_Stack.back().cursor.y += amount;
}

void UILayout::Separator()
{
    Spacing(4.0f);
}

void UILayout::BeginColumns(int32 count, float32 totalW)
{
    if (m_Stack.empty()) return;
    UILayoutState& s = m_Stack.back();
    float32 w = (totalW > 0) ? totalW : s.size.x;
    float32 colW = w / static_cast<float32>(count);
    s.columnWidths.resize(count, colW);
    s.currentColumn = 0;
}

void UILayout::NextColumn()
{
    if (m_Stack.empty()) return;
    UILayoutState& s = m_Stack.back();
    ++s.currentColumn;
    if (s.currentColumn < static_cast<int32>(s.columnWidths.size()))
    {
        s.sameLine = true;
        s.cursor.x += s.columnWidths[s.currentColumn - 1];
    }
}

void UILayout::EndColumns()
{
    if (!m_Stack.empty())
    {
        UILayoutState& s = m_Stack.back();
        s.columnWidths.clear();
        s.currentColumn = 0;
        NewLine();
    }
}

} // namespace Good