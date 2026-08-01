#pragma once
#include "Core/Core.h"
#include "Math/Rect.h"

namespace Good
{

    class EditorWindow
    {
    public:
        virtual ~EditorWindow() = default;

        virtual void OnRender() = 0;
        virtual void OnUpdate(float32 dt) {}
        virtual const char* GetTitle() const = 0;

        void SetRect(const Rect& r) { m_Rect = r; }
        void SetVisible(bool v)     { m_Visible = v; }

        const Rect& GetRect() const { return m_Rect;    }
        bool IsVisible()      const { return m_Visible; }

    protected:
        Rect m_Rect{100, 100, 400, 300};
        bool m_Visible{true};
        bool m_Modal{false};
    };

} // namespace Good