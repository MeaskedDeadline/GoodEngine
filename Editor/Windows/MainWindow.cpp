#include "Windows/MainWindow.h"
#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UILayout.h"
#include "UI/UIInput.h"
#include "UI/IconAtlas.h"
#include "UI/Widgets/UILabel.h"
#include "UI/Widgets/UIButton.h"
#include "Scene/SceneManager.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"
#include "Platform/Window.h"
#include "Core/Time.h"
#include "Core/Log.h"
#include "Core/UndoSystem.h"
#include <cstdio>
#include <cstring>

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

namespace Good
{

MainWindow::MainWindow() = default;

bool MainWindow::Initialize(uint32 w, uint32 h)
{
    m_ViewportW = w;
    m_ViewportH = h;
    m_Layout.Apply(w, h);
    LayoutPanels(w, h);
    return true;
}

void MainWindow::Shutdown() {}

void MainWindow::Resize(uint32 w, uint32 h)
{
    m_ViewportW = w;
    m_ViewportH = h;
    m_Layout.Apply(w, h);
    LayoutPanels(w, h);
}

void MainWindow::LayoutPanels(uint32, uint32)
{
    auto apply = [&](Panel& p, const char* n)
    {
        auto* r = m_Layout.GetRegion(n);
        if (r) p.SetRect(r->rect);
    };

    apply(m_Hierarchy, DefaultLayout::HIERARCHY);
    apply(m_Viewport,  DefaultLayout::VIEWPORT);
    apply(m_Inspector, DefaultLayout::INSPECTOR);
    apply(m_Content,   DefaultLayout::CONTENT);
    apply(m_Console,   DefaultLayout::CONSOLE);
}

void MainWindow::DrawTitleBar()
{
    auto& renderer = UIRenderer::Get();
    auto& ctx      = UIContext::Get();
    auto& input    = Input::Get();
    auto& icons    = IconAtlas::Get();

    float32 fullW   = static_cast<float32>(m_ViewportW);
    float32 barH    = 75.0f;
    float32 btnSize = 50.0f;
    float32 btnGap  = 4.0f;
    float32 startY  = (barH - btnSize) * 0.5f;

    // Фон
    renderer.DrawRect(Rect(0, 0, fullW, barH), Color(0.05f, 0.05f, 0.07f, 1.0f));
    renderer.DrawLine(Vec2(0, barH), Vec2(fullW, barH),
        Color(0.15f, 0.15f, 0.2f, 1.0f), 1.0f);

    float32 x = 8.0f;

    // Логотип G
    renderer.DrawRect(Rect(x, startY, btnSize, btnSize),
        Color(0.15f, 0.4f, 0.85f, 1.0f));
    renderer.DrawRectOutline(Rect(x, startY, btnSize, btnSize),
        Color(0.2f, 0.5f, 0.95f, 1.0f), 1.0f);
    renderer.DrawTextUI("G", Vec2(x + 16, startY + 14), Color::White(), 22.0f);
    x += btnSize + btnGap + 8.0f;

    // Кнопки инструментов с иконками
    auto drawIconBtn = [&](int32 iconIndex) -> bool
    {
        Rect r(x, startY, btnSize, btnSize);
        bool hovered = ctx.IsItemHovered(r);

        Color bg = hovered ? Color(0.22f, 0.22f, 0.28f, 1.0f)
                           : Color(0.12f, 0.12f, 0.15f, 1.0f);
        renderer.DrawRect(r, bg);
        renderer.DrawRectOutline(r, Color(0.25f, 0.25f, 0.3f, 1.0f), 1.0f);

        float32 iconSize = btnSize - 14.0f;
        float32 iconX = x + (btnSize - iconSize) * 0.5f;
        float32 iconY = startY + (btnSize - iconSize) * 0.5f;

        Color tint = hovered ? Color::White() : Color(0.8f, 0.8f, 0.8f, 1.0f);

        if (icons.IsReady())
            icons.DrawIcon(iconIndex, Rect(iconX, iconY, iconSize, iconSize), tint);

        x += btnSize + btnGap;
        return hovered && ctx.GetState().mousePressed;
    };

    if (drawIconBtn(IconAtlas::CURSOR))  {}
    if (drawIconBtn(IconAtlas::MOVE))    m_Viewport.SetGizmoMode(GizmoMode::Translate);
    if (drawIconBtn(IconAtlas::ROTATE))  m_Viewport.SetGizmoMode(GizmoMode::Rotate);
    if (drawIconBtn(IconAtlas::SCALE))   m_Viewport.SetGizmoMode(GizmoMode::Scale);

    // Window controls справа
    float32 ctrlBtnW = 46.0f;
    float32 ctrlBtnH = 32.0f;
    float32 ctrlY    = 4.0f;
    float32 ctrlX    = fullW - ctrlBtnW * 3 - 4;

    // Minimize
    {
        Rect r(ctrlX, ctrlY, ctrlBtnW, ctrlBtnH);
        bool hovered = ctx.IsItemHovered(r);
        if (hovered) renderer.DrawRect(r, Color(0.3f, 0.3f, 0.35f, 1.0f));
        renderer.DrawTextUI("-", Vec2(ctrlX + 18, ctrlY + 6),
            Color(0.8f, 0.8f, 0.8f, 1.0f), 18.0f);
        if (hovered && ctx.GetState().mousePressed)
        {
#if defined(GOOD_PLATFORM_WINDOWS)
            ShowWindow(static_cast<HWND>(Window::Get().GetNativeHandle()), SW_MINIMIZE);
#endif
        }
        ctrlX += ctrlBtnW;
    }

    // Maximize
    {
        Rect r(ctrlX, ctrlY, ctrlBtnW, ctrlBtnH);
        bool hovered = ctx.IsItemHovered(r);
        if (hovered) renderer.DrawRect(r, Color(0.3f, 0.3f, 0.35f, 1.0f));
        renderer.DrawTextUI("[]", Vec2(ctrlX + 14, ctrlY + 6),
            Color(0.8f, 0.8f, 0.8f, 1.0f), 16.0f);
        if (hovered && ctx.GetState().mousePressed)
        {
#if defined(GOOD_PLATFORM_WINDOWS)
            HWND hwnd = static_cast<HWND>(Window::Get().GetNativeHandle());
            if (IsZoomed(hwnd)) ShowWindow(hwnd, SW_RESTORE);
            else                ShowWindow(hwnd, SW_MAXIMIZE);
#endif
        }
        ctrlX += ctrlBtnW;
    }

    // Close
    {
        Rect r(ctrlX, ctrlY, ctrlBtnW, ctrlBtnH);
        bool hovered = ctx.IsItemHovered(r);
        renderer.DrawRect(r, hovered ? Color(0.85f, 0.15f, 0.15f, 1.0f)
                                     : Color(0.5f, 0.1f, 0.1f, 1.0f));
        renderer.DrawTextUI("X", Vec2(ctrlX + 16, ctrlY + 6), Color::White(), 18.0f);
        if (hovered && ctx.GetState().mousePressed)
            Engine::Get().Quit();
    }

    // Drag window
    float32 dragEndX = fullW - ctrlBtnW * 3 - 8;
    Rect dragArea(x + 20, 0, dragEndX - x - 20, barH);

    if (ctx.IsItemHovered(dragArea) && input.IsMouseButtonPressed(MouseButton::Left))
    {
#if defined(GOOD_PLATFORM_WINDOWS)
        HWND hwnd = static_cast<HWND>(Window::Get().GetNativeHandle());
        ReleaseCapture();
        SendMessageA(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
#endif
    }

    // Double click maximize
    if (ctx.IsItemHovered(dragArea))
    {
        static float64 lastClick = 0;
        if (input.IsMouseButtonPressed(MouseButton::Left))
        {
            float64 now = Time::Elapsed();
            if (now - lastClick < 0.3)
            {
#if defined(GOOD_PLATFORM_WINDOWS)
                HWND hwnd = static_cast<HWND>(Window::Get().GetNativeHandle());
                if (IsZoomed(hwnd)) ShowWindow(hwnd, SW_RESTORE);
                else                ShowWindow(hwnd, SW_MAXIMIZE);
#endif
            }
            lastClick = now;
        }
    }
}

void MainWindow::DrawMenuBar()
{
    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();
    auto& ctx      = UIContext::Get();

    float32 fullW = static_cast<float32>(m_ViewportW);
    float32 menuY = 75.0f;

    renderer.DrawRect(Rect(0, menuY, fullW, 24.0f), Color(0.09f, 0.09f, 0.11f, 1.0f));

    float32 x = 6.0f;
    const float32 y = menuY + 2.0f, btnH = 20.0f, gap = 1.0f;

    struct MI { const char* label; float32 width; };
    MI items[] = {{"File",48},{"Edit",48},{"View",48},{"Build",52},{"Window",64},{"Help",48}};

    for (auto& item : items)
    {
        Rect btnRect(x, y, item.width, btnH);
        bool hovered = ctx.IsItemHovered(btnRect);
        if (hovered)
            renderer.DrawRect(btnRect, Color(0.2f, 0.2f, 0.25f, 1.0f), 2.0f);
        renderer.DrawTextUI(item.label, Vec2(x + 6, y + 2),
            hovered ? Color::White() : Color(0.7f, 0.7f, 0.75f, 1.0f), 12.0f);
        if (std::strcmp(item.label, "File") == 0 && hovered && ctx.GetState().mousePressed)
            m_ShowFileMenu = !m_ShowFileMenu;
        else if (hovered && ctx.GetState().mousePressed)
            m_ShowFileMenu = false;
        x += item.width + gap;
    }

    char fps[32];
    std::snprintf(fps, sizeof(fps), "%.0f FPS", static_cast<double>(Time::FPS()));
    renderer.DrawTextUI(fps, Vec2(fullW - 80, y + 2),
        Color(0.5f, 0.5f, 0.55f, 1.0f), 11.0f);

    if (m_ShowFileMenu)
    {
        float32 menuX = 6.0f, dropY = menuY + 24.0f;
        float32 menuW = 200.0f, itemH = 26.0f;
        float32 menuH = itemH * 4 + 12.0f;

        Rect dropRect(menuX, dropY, menuW, menuH);
        renderer.DrawRect(dropRect, Color(0.12f, 0.12f, 0.15f, 0.98f));
        renderer.DrawRectOutline(dropRect, style.colors.border, 1.0f);

        float32 iy = dropY + 4;
        auto drawItem = [&](const char* label, const char* sc = "") -> bool
        {
            Rect ir(menuX + 2, iy, menuW - 4, itemH);
            bool h = ctx.IsItemHovered(ir);
            if (h) renderer.DrawRect(ir, style.colors.selection, 2.0f);
            renderer.DrawTextUI(label, Vec2(menuX + 12, iy + 5),
                h ? Color::White() : style.colors.text, 12.0f);
            if (sc[0])
            {
                float32 sw = static_cast<float32>(std::strlen(sc)) * 8.0f;
                renderer.DrawTextUI(sc, Vec2(menuX + menuW - sw - 12, iy + 5),
                    style.colors.textDisabled, 11.0f);
            }
            iy += itemH;
            return h && ctx.GetState().mousePressed;
        };

        if (drawItem("New Scene", "Ctrl+N"))  { if (m_OnNewScene)  m_OnNewScene();  m_ShowFileMenu = false; }
        if (drawItem("Save Scene", "Ctrl+S")) { if (m_OnSaveScene) m_OnSaveScene(); m_ShowFileMenu = false; }
        if (drawItem("Load Scene", "Ctrl+O")) { if (m_OnLoadScene) m_OnLoadScene(); m_ShowFileMenu = false; }
        iy += 4;
        if (drawItem("Quit", "Esc")) { Engine::Get().Quit(); m_ShowFileMenu = false; }

        if (ctx.GetState().mousePressed && !ctx.IsItemHovered(dropRect))
            m_ShowFileMenu = false;
    }
}

void MainWindow::DrawToolbar()
{
    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();

    float32 fullW = static_cast<float32>(m_ViewportW);
    float32 toolY = 99.0f;

    renderer.DrawRect(Rect(0, toolY, fullW, 44.0f), Color(0.11f, 0.11f, 0.14f, 1.0f));
    renderer.DrawLine(Vec2(0, toolY + 44), Vec2(fullW, toolY + 44),
        style.colors.border, 1.0f);

    const float32 y = toolY + 8.0f, btnH = 30.0f, gap = 3.0f;
    float32 x = 8.0f;

    auto drawBtn = [&](const char* label, float32 w)
    {
        UILayout::Get().PushLayout(Rect(x, y, w, btnH));
        UIButton::Draw(label, Vec2(w, btnH));
        UILayout::Get().PopLayout();
        x += w + gap;
    };

    auto drawSep = [&]()
    {
        renderer.DrawLine(Vec2(x + 4, toolY + 6), Vec2(x + 4, toolY + 38),
            style.colors.separator, 1.0f);
        x += 12;
    };

    drawBtn("Save", 55);
    drawBtn("Open", 55);
    drawSep();
    drawBtn("Move", 55);
    drawBtn("Rot", 50);
    drawBtn("Scale", 60);
    drawSep();
    drawBtn("World", 60);
    drawSep();
    drawBtn("Play", 55);
    drawBtn("Pause", 60);
    drawBtn("Stop", 55);
}

void MainWindow::OnUpdate(float32 dt)
{
    m_Hierarchy.HandleDragResize();
    m_Inspector.HandleDragResize();
    m_Content.HandleDragResize();
    m_Console.HandleDragResize();
    m_Viewport.HandleDragResize();

    m_Hierarchy.OnUpdate(dt);
    m_Inspector.OnUpdate(dt);
    m_Content.OnUpdate(dt);
    m_Viewport.OnUpdate(dt);
    m_Profiler.OnUpdate(dt);

    auto hierSel = m_Hierarchy.GetSelectedNode();
    auto vpSel   = m_Viewport.GetSelectedNode();
    Ref<SceneNode> selected = vpSel ? vpSel : hierSel;
    m_Inspector.SetSelectedNode(selected);
    m_Viewport.SetSelectedNode(selected);

    auto& input = Input::Get();
    if (input.IsKeyDown(Key::LeftCtrl))
    {
        if (input.IsKeyPressed(Key::N) && m_OnNewScene)  { m_OnNewScene();  m_ShowFileMenu = false; }
        if (input.IsKeyPressed(Key::S) && m_OnSaveScene) { m_OnSaveScene(); m_ShowFileMenu = false; }
        if (input.IsKeyPressed(Key::O) && m_OnLoadScene) { m_OnLoadScene(); m_ShowFileMenu = false; }
        if (input.IsKeyPressed(Key::Z)) UndoSystem::Get().Undo();
        if (input.IsKeyPressed(Key::Y)) UndoSystem::Get().Redo();
    }
}

void MainWindow::OnRender()
{
    auto& ctx = UIContext::Get();
    ctx.BeginFrame();

    DrawToolbar();

    m_Hierarchy.OnRender();
    m_Viewport.OnRender();
    m_Inspector.OnRender();
    m_Content.OnRender();
    m_Console.OnRender();

    if (m_ShowProfiler) m_Profiler.OnRender();

    DrawMenuBar();
    DrawTitleBar();

    ctx.EndFrame();
}

} // namespace Good