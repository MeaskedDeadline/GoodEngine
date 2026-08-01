#pragma once
#include "../../Core/Core.h"
#include "../../Math/Vec2.h"
#include "../../Math/Rect.h"
#include "../../Math/Color.h"

namespace Good
{

    class UIWidget
    {
    public:
        virtual ~UIWidget() = default;

        virtual void Render() = 0;
        virtual bool HandleInput() { return false; }

        void SetRect(const Rect& r)         { m_Rect = r; }
        void SetEnabled(bool e)             { m_Enabled = e; }
        void SetVisible(bool v)             { m_Visible = v; }
        void SetTooltip(const std::string& t){ m_Tooltip = t; }

        const Rect& GetRect()   const { return m_Rect;    }
        bool IsEnabled()        const { return m_Enabled; }
        bool IsVisible()        const { return m_Visible; }

    protected:
        Rect        m_Rect{0,0,100,24};
        bool        m_Enabled{true};
        bool        m_Visible{true};
        std::string m_Tooltip;
    };

} // namespace Good