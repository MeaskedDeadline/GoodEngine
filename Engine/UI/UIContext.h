#pragma once
#include "Core/Core.h"
#include "Math/Vec2.h"
#include "Math/Rect.h"
#include "UI/UIStyle.h"

namespace Good
{

    struct UIState
    {
        uint64  hoveredID{0};
        uint64  activeID{0};
        uint64  focusedID{0};
        Vec2    mousePos{0,0};
        bool    mouseDown{false};
        bool    mousePressed{false};
        bool    mouseReleased{false};
    };

    class UIContext : public NonCopyable
    {
    public:
        static UIContext& Get();

        bool Initialize(uint32 viewportW, uint32 viewportH);
        void Shutdown();

        void BeginFrame();
        void EndFrame();

        void Resize(uint32 w, uint32 h);

        UIStyle& GetStyle() { return m_Style; }
        UIState& GetState() { return m_State; }

        uint64 GenerateID(const std::string& label);
        uint64 GenerateID(const std::string& label, const void* ptr);

        bool IsHovered(uint64 id) const { return m_State.hoveredID == id; }
        bool IsActive(uint64 id)  const { return m_State.activeID  == id; }
        bool IsFocused(uint64 id) const { return m_State.focusedID == id; }

        void SetHovered(uint64 id) { m_State.hoveredID = id; }
        void SetActive(uint64 id)  { m_State.activeID  = id; }
        void SetFocused(uint64 id) { m_State.focusedID = id; }

        bool IsItemHovered(const Rect& rect) const;

        uint32 GetViewportW() const { return m_ViewportW; }
        uint32 GetViewportH() const { return m_ViewportH; }

    private:
        UIContext() = default;

        UIStyle m_Style;
        UIState m_State;
        uint32  m_ViewportW{1280};
        uint32  m_ViewportH{720};
    };

} // namespace Good