#pragma once
#include "Core/Core.h"
#include "Math/Rect.h"
#include "Math/Vec2.h"

namespace Good
{

    class Panel
    {
    public:
        virtual ~Panel() = default;

        virtual void OnRender() = 0;
        virtual void OnUpdate(float32 dt) {}
        virtual const char* GetTitle() const = 0;

        void SetRect(const Rect& r) { m_Rect = r; }
        void SetVisible(bool v)     { m_Visible = v; }
        void SetFocused(bool f)     { m_Focused = f; }

        const Rect& GetRect() const { return m_Rect;    }
        bool IsVisible()      const { return m_Visible; }
        bool IsFocused()      const { return m_Focused; }

        // Обработка drag и resize — вызывается в MainWindow
        void HandleDragResize();

        bool IsDragging()  const { return m_Dragging;  }
        bool IsResizing()  const { return m_Resizing;  }

    protected:
        // Рисует title bar и фон панели
        void DrawPanelFrame();

        Rect    m_Rect{0, 0, 200, 300};
        bool    m_Visible{true};
        bool    m_Focused{false};

        // Drag
        bool    m_Dragging{false};
        float32 m_DragOffsetX{0};
        float32 m_DragOffsetY{0};

        // Resize
        bool    m_Resizing{false};
        int32   m_ResizeEdge{0}; // 1=right, 2=bottom, 3=corner
        float32 m_ResizeStartW{0};
        float32 m_ResizeStartH{0};
        float32 m_ResizeStartX{0};
        float32 m_ResizeStartY{0};

        static constexpr float32 TITLE_BAR_H  = 26.0f;
        static constexpr float32 MIN_WIDTH    = 150.0f;
        static constexpr float32 MIN_HEIGHT   = 100.0f;
        static constexpr float32 RESIZE_EDGE  = 6.0f;
    };

} // namespace Good