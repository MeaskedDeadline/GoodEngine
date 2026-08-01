#pragma once
#include "Core/Core.h"
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "GamepadCodes.h"

namespace Good
{

    class Input : public NonCopyable
    {
    public:
        static Input& Get();

        void Initialize();
        void Shutdown();
        void BeginFrame();
        void EndFrame();

        void OnKeyEvent(int32 platformKey, bool pressed);
        void OnMouseButtonEvent(int32 button, bool pressed);
        void OnMouseMove(float32 x, float32 y);
        void OnMouseScroll(float32 dx, float32 dy);

        bool IsKeyDown(Key key)     const;
        bool IsKeyPressed(Key key)  const;
        bool IsKeyReleased(Key key) const;

        bool IsMouseButtonDown(MouseButton btn)     const;
        bool IsMouseButtonPressed(MouseButton btn)  const;
        bool IsMouseButtonReleased(MouseButton btn) const;

        float32 GetMouseX()      const { return m_MouseX;      }
        float32 GetMouseY()      const { return m_MouseY;      }
        float32 GetMouseDeltaX() const { return m_MouseDeltaX; }
        float32 GetMouseDeltaY() const { return m_MouseDeltaY; }
        float32 GetScrollX()     const { return m_ScrollX;     }
        float32 GetScrollY()     const { return m_ScrollY;     }

        // Сброс дельты и last positions — использовать после SetCursorPos
        void ResetMouseDelta();

    private:
        Input() = default;

        static constexpr int32 KeyCount    = static_cast<int32>(Key::Count);
        static constexpr int32 ButtonCount = static_cast<int32>(MouseButton::Count);

        bool    m_KeyCurrent[KeyCount]        = {};
        bool    m_KeyPrevious[KeyCount]       = {};
        bool    m_ButtonCurrent[ButtonCount]  = {};
        bool    m_ButtonPrevious[ButtonCount] = {};

        float32 m_MouseX{0}, m_MouseY{0};
        float32 m_LastMouseX{0}, m_LastMouseY{0};
        float32 m_MouseDeltaX{0}, m_MouseDeltaY{0};
        float32 m_ScrollX{0}, m_ScrollY{0};
        bool    m_FirstMouse{true};
    };

} // namespace Good