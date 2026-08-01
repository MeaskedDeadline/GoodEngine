#pragma once
#include "Core/Core.h"
#include "Math/Vec2.h"

namespace Good
{

    class UIInput
    {
    public:
        static UIInput& Get();

        void BeginFrame();
        void EndFrame();

        void OnMouseMove(float32 x, float32 y);
        void OnMouseButton(int32 button, bool pressed);
        void OnMouseScroll(float32 dx, float32 dy);
        void OnChar(uint32 codepoint);
        void OnKey(int32 keyCode, bool pressed);

        float32 GetMouseX()  const { return m_MouseX;  }
        float32 GetMouseY()  const { return m_MouseY;  }
        float32 GetScrollX() const { return m_ScrollX; }
        float32 GetScrollY() const { return m_ScrollY; }

        bool IsMouseDown(int32 btn)     const;
        bool IsMousePressed(int32 btn)  const;
        bool IsMouseReleased(int32 btn) const;
        bool IsKeyDown(int32 key)       const;
        bool IsKeyPressed(int32 key)    const;
        bool IsKeyReleased(int32 key)   const;

        const std::string& GetInputText() const { return m_InputText; }
        void ClearInputText()                   { m_InputText.clear(); }

    private:
        UIInput() = default;

        static constexpr int32 ButtonCount = 8;
        static constexpr int32 KeyCount    = 512;

        bool    m_MouseCurrent[ButtonCount]  = {};
        bool    m_MousePrevious[ButtonCount] = {};
        bool    m_KeyCurrent[KeyCount]       = {};
        bool    m_KeyPrevious[KeyCount]      = {};

        float32 m_MouseX{0}, m_MouseY{0};
        float32 m_ScrollX{0}, m_ScrollY{0};
        std::string m_InputText;
    };

} // namespace Good