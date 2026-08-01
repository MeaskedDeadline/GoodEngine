#include "Panels/ContentBrowser.h"
#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UILayout.h"
#include "UI/Widgets/UILabel.h"
#include "UI/Widgets/UIButton.h"
#include "UI/Widgets/UITextInput.h"
#include "FileSystem/FileSystem.h"

namespace Good
{

ContentBrowser::ContentBrowser()
{
    m_CurrentPath = m_RootPath;
}

void ContentBrowser::SetRootPath(const Path& path)
{
    m_RootPath     = path;
    m_CurrentPath  = path;
    m_NeedsRefresh = true;
}

void ContentBrowser::OnUpdate(float32)
{
    if (m_NeedsRefresh)
    {
        m_Items.clear();
        if (FileSystem::IsDirectory(m_CurrentPath))
            m_Items = FileSystem::ListDirectory(m_CurrentPath);
        m_NeedsRefresh = false;
    }
}

void ContentBrowser::NavigateTo(const Path& path)
{
    m_History.push_back(m_CurrentPath);
    m_CurrentPath  = path;
    m_NeedsRefresh = true;
}

void ContentBrowser::NavigateBack()
{
    if (!m_History.empty())
    {
        m_CurrentPath  = m_History.back();
        m_History.pop_back();
        m_NeedsRefresh = true;
    }
}

void ContentBrowser::DrawToolbar()
{
    auto& style    = UIContext::Get().GetStyle();
    auto& renderer = UIRenderer::Get();

    Rect bar(m_Rect.x, m_Rect.y + 28, m_Rect.width, 34.0f);
    renderer.DrawRect(bar, style.colors.header);

    float32 y = m_Rect.y + 34;
    float32 x = m_Rect.x + 8;

    auto drawBtn = [&](const char* label, float32 w) -> bool
    {
        UILayout::Get().PushLayout(Rect(x, y, w, 24));
        bool clicked = UIButton::Draw(label, Vec2(w, 24));
        UILayout::Get().PopLayout();
        x += w + 3;
        return clicked;
    };

    if (drawBtn("Refresh", 65)) m_NeedsRefresh = true;
    if (drawBtn("Back",    50) && !m_History.empty()) NavigateBack();
    x += 8;

    UILayout::Get().PushLayout(Rect(x, y,
        m_Rect.width - (x - m_Rect.x) - 8, 24));
    UITextInput::Draw("", m_SearchFilter, "Filter...", 200);
    UILayout::Get().PopLayout();
}

void ContentBrowser::DrawBreadcrumb()
{
    auto& style    = UIContext::Get().GetStyle();
    auto& renderer = UIRenderer::Get();

    renderer.DrawRect(
        Rect(m_Rect.x, m_Rect.y + 62, m_Rect.width, 22),
        Color(0.10f, 0.10f, 0.13f, 1.0f));

    std::string path = "Path: " + m_CurrentPath.ToString();
    renderer.DrawTextUI(path,
        Vec2(m_Rect.x + 10, m_Rect.y + 66),
        style.colors.textDisabled, 11.0f);
}

void ContentBrowser::DrawItems()
{
    float32 startY = m_Rect.y + 90;
    Rect contentRect(m_Rect.x + 8, startY,
                     m_Rect.width - 16, m_Rect.height - 100);
    UILayout::Get().PushLayout(contentRect);

    if (m_Items.empty())
    {
        UILabel::DrawColored("Empty folder",
            UIContext::Get().GetStyle().colors.textDisabled);
    }
    else
    {
        for (auto& item : m_Items)
        {
            std::string name = item.GetFilename();
            if (!m_SearchFilter.empty() &&
                name.find(m_SearchFilter) == std::string::npos) continue;

            bool isDir = FileSystem::IsDirectory(item);
            DrawItem(item, isDir);
        }
    }

    UILayout::Get().PopLayout();
}

void ContentBrowser::DrawItem(const Path& path, bool isDir)
{
    auto& ctx      = UIContext::Get();
    auto& renderer = UIRenderer::Get();
    auto& style    = ctx.GetStyle();

    Vec2 cursor = UILayout::Get().GetCursor();
    Rect rect(cursor.x, cursor.y, m_Rect.width - 20, 24);

    bool selected = (path == m_SelectedItem);
    bool hovered  = ctx.IsItemHovered(rect);

    Color bg = selected ? style.colors.selection
             : hovered  ? style.colors.backgroundHovered
             :             Color(0,0,0,0);

    if (bg.a > 0.01f)
        renderer.DrawRect(rect, bg, 2.0f);

    std::string prefix = isDir ? "[D] " : "[F] ";
    std::string name   = prefix + path.GetFilename();

    Color textColor = isDir ? Color(0.9f, 0.75f, 0.2f, 1.0f) : style.colors.text;
    renderer.DrawTextUI(name,
        Vec2(rect.x + 6, rect.y + 5),
        textColor, 12.0f);

    if (hovered && ctx.GetState().mousePressed)
    {
        m_SelectedItem = path;
        if (isDir) NavigateTo(path);
    }

    UILayout::Get().NextItemRect(rect.width, 26);
}

void ContentBrowser::OnRender()
{
    if (!m_Visible) return;

    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();

    renderer.DrawRect(m_Rect, style.colors.windowBackground);
    renderer.DrawRectOutline(m_Rect, style.colors.border, 1.0f);

    Rect headerRect(m_Rect.x, m_Rect.y, m_Rect.width, 28.0f);
    renderer.DrawRect(headerRect, style.colors.titleBar);
    renderer.DrawTextUI("CONTENT BROWSER",
        Vec2(m_Rect.x + 12, m_Rect.y + 7),
        style.colors.text, 14.0f);

    DrawToolbar();
    DrawBreadcrumb();
    DrawItems();
}

} // namespace Good