#include "Input.h"
#include "Core/Log.h"
#include <cstring>

namespace Good
{

Input& Input::Get()
{
    static Input s_Instance;
    return s_Instance;
}

void Input::Initialize()
{
    std::memset(m_KeyCurrent,     0, sizeof(m_KeyCurrent));
    std::memset(m_KeyPrevious,    0, sizeof(m_KeyPrevious));
    std::memset(m_ButtonCurrent,  0, sizeof(m_ButtonCurrent));
    std::memset(m_ButtonPrevious, 0, sizeof(m_ButtonPrevious));
    GOOD_LOG_INFO("Input", "Input initialized");
}

void Input::Shutdown() { GOOD_LOG_INFO("Input", "Input shutdown"); }

void Input::BeginFrame()
{
    std::memcpy(m_KeyPrevious,    m_KeyCurrent,    sizeof(m_KeyCurrent));
    std::memcpy(m_ButtonPrevious, m_ButtonCurrent, sizeof(m_ButtonCurrent));

    if (m_FirstMouse)
    {
        m_LastMouseX = m_MouseX;
        m_LastMouseY = m_MouseY;
        m_FirstMouse = false;
    }

    m_MouseDeltaX = m_MouseX - m_LastMouseX;
    m_MouseDeltaY = m_MouseY - m_LastMouseY;
    m_LastMouseX  = m_MouseX;
    m_LastMouseY  = m_MouseY;
    m_ScrollX = m_ScrollY = 0.0f;
}

void Input::EndFrame() {}

void Input::OnKeyEvent(int32 key, bool pressed)
{
    if (key >= 0 && key < KeyCount) m_KeyCurrent[key] = pressed;
}

void Input::OnMouseButtonEvent(int32 btn, bool pressed)
{
    if (btn >= 0 && btn < ButtonCount) m_ButtonCurrent[btn] = pressed;
}

void Input::OnMouseMove(float32 x, float32 y)
{
    m_MouseX = x;
    m_MouseY = y;
}

void Input::OnMouseScroll(float32 dx, float32 dy)
{
    m_ScrollX += dx;
    m_ScrollY += dy;
}

// Сброс дельты — вызывать после того как поставили курсор SetCursorPos
void Input::ResetMouseDelta()
{
    m_LastMouseX  = m_MouseX;
    m_LastMouseY  = m_MouseY;
    m_MouseDeltaX = 0.0f;
    m_MouseDeltaY = 0.0f;
}

bool Input::IsKeyDown(Key key)     const { int32 i = static_cast<int32>(key); return (i >= 0 && i < KeyCount)    ? m_KeyCurrent[i]    : false; }
bool Input::IsKeyPressed(Key key)  const { int32 i = static_cast<int32>(key); return (i >= 0 && i < KeyCount)    ? (m_KeyCurrent[i]  && !m_KeyPrevious[i])  : false; }
bool Input::IsKeyReleased(Key key) const { int32 i = static_cast<int32>(key); return (i >= 0 && i < KeyCount)    ? (!m_KeyCurrent[i] && m_KeyPrevious[i])   : false; }

bool Input::IsMouseButtonDown(MouseButton btn)     const { int32 i = static_cast<int32>(btn); return (i >= 0 && i < ButtonCount) ? m_ButtonCurrent[i]                              : false; }
bool Input::IsMouseButtonPressed(MouseButton btn)  const { int32 i = static_cast<int32>(btn); return (i >= 0 && i < ButtonCount) ? (m_ButtonCurrent[i]  && !m_ButtonPrevious[i])  : false; }
bool Input::IsMouseButtonReleased(MouseButton btn) const { int32 i = static_cast<int32>(btn); return (i >= 0 && i < ButtonCount) ? (!m_ButtonCurrent[i] && m_ButtonPrevious[i])   : false; }

} // namespace Good