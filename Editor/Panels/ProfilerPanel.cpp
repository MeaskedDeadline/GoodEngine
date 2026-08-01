#include "Panels/ProfilerPanel.h"
#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UILayout.h"
#include "UI/Widgets/UILabel.h"
#include "UI/Widgets/UIButton.h"
#include "UI/Widgets/UIProgressBar.h"
#include "Platform/Platform.h"
#include "Math/MathCommon.h"
#include <algorithm>
#include <cstdio>

namespace Good
{

ProfilerPanel::ProfilerPanel() = default;

void ProfilerPanel::OnUpdate(float32 dt)
{
    m_UpdateTimer += dt;
    if (m_UpdateTimer >= m_UpdateInterval)
    {
        m_UpdateTimer = 0;
        m_CachedEntries.clear();
        for (auto& [name, entry] : Profiler::Get().GetEntries())
            m_CachedEntries.push_back({name, entry});

        std::sort(m_CachedEntries.begin(), m_CachedEntries.end(),
            [](auto& a, auto& b) { return a.second.avgMs > b.second.avgMs; });
    }
}

void ProfilerPanel::OnRender()
{
    if (!m_Visible) return;

    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();

    renderer.DrawRect(m_Rect, style.colors.windowBackground);
    renderer.DrawRectOutline(m_Rect, style.colors.border, 1.0f);

    Rect headerRect(m_Rect.x, m_Rect.y, m_Rect.width, 28.0f);
    renderer.DrawRect(headerRect, style.colors.titleBar);
    renderer.DrawTextUI("PROFILER",
        Vec2(m_Rect.x + 12, m_Rect.y + 7),
        style.colors.text, 14.0f);

    Rect content(m_Rect.x + 8, m_Rect.y + 38,
                 m_Rect.width - 16, m_Rect.height - 46);
    UILayout::Get().PushLayout(content);

    char buf[128];
    float64 frameMs = Profiler::Get().GetFrameMs();
    float32 fps     = Profiler::Get().GetFPS();

    std::snprintf(buf, sizeof(buf), "Frame: %.2fms  FPS: %.0f",
        frameMs, static_cast<double>(fps));
    UILabel::DrawColored(buf, Color(0.6f, 0.9f, 0.6f, 1.0f));
    UILayout::Get().Spacing(4);

    UIProgressBar::Draw(Math::Clamp(fps / 120.0f, 0.0f, 1.0f),
        Vec2(m_Rect.width - 32, 12), "");
    UILayout::Get().Spacing(8);

    uint64 memUsage = Platform::GetMemoryUsage();
    std::snprintf(buf, sizeof(buf), "Memory: %llu KB",
        static_cast<unsigned long long>(memUsage / 1024));
    UILabel::DrawColored(buf, style.colors.textDisabled);
    UILayout::Get().Spacing(8);

    UILabel::DrawColored("Samples:", style.colors.text);
    UILayout::Get().Spacing(4);

    float32 maxMs = 0.0f;
    for (auto& [name, entry] : m_CachedEntries)
        maxMs = Math::Max(maxMs, static_cast<float32>(entry.avgMs));

    for (auto& [name, entry] : m_CachedEntries)
    {
        char line[128];
        std::snprintf(line, sizeof(line), "%s: %.3fms", name.c_str(), entry.avgMs);
        UILabel::DrawColored(line, style.colors.text);
    }

    UILayout::Get().PopLayout();
}

} // namespace Good