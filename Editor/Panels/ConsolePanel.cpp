#include "Panels/ConsolePanel.h"
#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UILayout.h"
#include "UI/Widgets/UILabel.h"
#include "UI/Widgets/UIButton.h"
#include "UI/Widgets/UICheckbox.h"
#include "UI/Widgets/UITextInput.h"

namespace Good
{

ConsolePanel::ConsolePanel() = default;

void ConsolePanel::DrawToolbar()
{
    auto& style    = UIContext::Get().GetStyle();
    auto& renderer = UIRenderer::Get();

    Rect bar(m_Rect.x, m_Rect.y + 28, m_Rect.width, 30.0f);
    renderer.DrawRect(bar, style.colors.header);

    float32 y = m_Rect.y + 32;
    float32 x = m_Rect.x + 8;

    UILayout::Get().PushLayout(Rect(x, y, 60, 22));
    if (UIButton::Draw("Clear", Vec2(60, 22))) Clear();
    UILayout::Get().PopLayout();
    x += 68;

    UILayout::Get().PushLayout(Rect(x, y, 220, 22));
    UICheckbox::Draw("Info",  m_ShowInfo);
    UILayout::Get().SameLine(4);
    UICheckbox::Draw("Warn",  m_ShowWarn);
    UILayout::Get().SameLine(4);
    UICheckbox::Draw("Error", m_ShowError);
    UILayout::Get().PopLayout();
}

void ConsolePanel::DrawEntries()
{
    auto& entries = DebugConsole::Get().GetEntries();
    auto& style   = UIContext::Get().GetStyle();

    float32 startY   = m_Rect.y + 62;
    float32 viewH    = m_Rect.height - 92;
    float32 entryH   = 18.0f;
    float32 contentH = static_cast<float32>(entries.size()) * entryH;

    if (m_AutoScroll && contentH > viewH)
        m_ScrollY = contentH - viewH;

    Rect content(m_Rect.x + 8, startY - m_ScrollY,
                 m_Rect.width - 16, contentH);
    UILayout::Get().PushLayout(content);

    for (auto& entry : entries)
    {
        bool show = false;
        switch (entry.level)
        {
            case LogLevel::Trace: show = m_ShowTrace; break;
            case LogLevel::Info:  show = m_ShowInfo;  break;
            case LogLevel::Warn:  show = m_ShowWarn;  break;
            case LogLevel::Error:
            case LogLevel::Fatal: show = m_ShowError; break;
            default: show = true; break;
        }
        if (!show) continue;

        Color col = style.colors.text;
        switch (entry.level)
        {
            case LogLevel::Trace: col = style.colors.textDisabled;       break;
            case LogLevel::Info:  col = Color(0.6f, 0.9f, 0.6f, 1.0f); break;
            case LogLevel::Warn:  col = Color(0.95f,0.85f,0.4f,1.0f);  break;
            case LogLevel::Error:
            case LogLevel::Fatal: col = Color(0.95f,0.5f,0.5f,1.0f);   break;
            default: break;
        }

        std::string line = "[" + entry.category + "] " + entry.message;
        UILabel::DrawColored(line, col);
    }

    UILayout::Get().PopLayout();
}

void ConsolePanel::DrawCommandInput()
{
    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();

    Rect inputRect(m_Rect.x, m_Rect.y + m_Rect.height - 30,
                   m_Rect.width, 30.0f);
    renderer.DrawRect(inputRect, style.colors.header);

    // Поле ввода — с обработкой Enter
    UILayout::Get().PushLayout(Rect(inputRect.x + 8, inputRect.y + 4,
                                    inputRect.width - 80, 22));
    bool enterPressed = UITextInput::DrawWithEnter("", m_CommandInput,
        "Type command (help, clear, scene.info, time.info)...",
        inputRect.width - 80);
    UILayout::Get().PopLayout();

    // Кнопка Run
    UILayout::Get().PushLayout(Rect(inputRect.x + inputRect.width - 65,
                                    inputRect.y + 4, 55, 22));
    bool runClicked = UIButton::Draw("Run", Vec2(55, 22));
    UILayout::Get().PopLayout();

    // Выполнить команду при Enter или клике Run
    if ((enterPressed || runClicked) && !m_CommandInput.empty())
    {
        DebugConsole::Get().ExecuteCommand(m_CommandInput);
        m_CommandInput.clear();
    }
}

void ConsolePanel::OnRender()
{
    if (!m_Visible) return;

    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();

    renderer.DrawRect(m_Rect, style.colors.windowBackground);
    renderer.DrawRectOutline(m_Rect, style.colors.border, 1.0f);

    Rect headerRect(m_Rect.x, m_Rect.y, m_Rect.width, 28.0f);
    renderer.DrawRect(headerRect, style.colors.titleBar);
    renderer.DrawTextUI("CONSOLE",
        Vec2(m_Rect.x + 12, m_Rect.y + 7),
        style.colors.text, 14.0f);

    DrawToolbar();
    DrawEntries();
    DrawCommandInput();
}

} // namespace Good