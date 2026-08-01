#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UILayout.h"
#include "UI/UIInput.h"
#include "Core/Log.h"
#include "Utilities/Hash.h"

namespace Good
{

    UIContext& UIContext::Get()
    {
        static UIContext s_Instance;
        return s_Instance;
    }

    bool UIContext::Initialize(uint32 w, uint32 h)
    {
        m_ViewportW = w;
        m_ViewportH = h;
        UIRenderer::Get().Initialize(w, h);
        GOOD_LOG_INFO("UIContext", "Initialized ({}x{})", w, h);
        return true;
    }

    void UIContext::Shutdown()
    {
        UIRenderer::Get().Shutdown();
        GOOD_LOG_INFO("UIContext", "Shutdown");
    }

    void UIContext::BeginFrame()
    {
        // ВАЖНО: НЕ вызываем UIInput::BeginFrame() тут!
        // Он должен вызываться ДО обновления событий, в EditorLayer::Update

        UIRenderer::Get().BeginFrame();

        // Читаем состояние из UIInput
        m_State.mousePos     = Vec2(UIInput::Get().GetMouseX(), UIInput::Get().GetMouseY());
        m_State.mouseDown    = UIInput::Get().IsMouseDown(0);
        m_State.mousePressed = UIInput::Get().IsMousePressed(0);
        m_State.mouseReleased= UIInput::Get().IsMouseReleased(0);

        m_State.hoveredID = 0;
    }

    void UIContext::EndFrame()
    {
        UIRenderer::Get().EndFrame();

        if (!m_State.mouseDown)
            m_State.activeID = 0;
    }

    void UIContext::Resize(uint32 w, uint32 h)
    {
        m_ViewportW = w;
        m_ViewportH = h;
        UIRenderer::Get().Resize(w, h);
    }

    uint64 UIContext::GenerateID(const std::string& label)
    {
        return Hash::String64(label);
    }

    uint64 UIContext::GenerateID(const std::string& label, const void* ptr)
    {
        uint64 h1 = Hash::String64(label);
        uint64 h2 = reinterpret_cast<uint64>(ptr);
        return Hash::Combine64(h1, h2);
    }

    bool UIContext::IsItemHovered(const Rect& rect) const
    {
        Vec2 mp = m_State.mousePos;
        return mp.x >= rect.x && mp.x <= rect.x + rect.width
            && mp.y >= rect.y && mp.y <= rect.y + rect.height;
    }

} // namespace Good