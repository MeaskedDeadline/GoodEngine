#include "UI/UIInput.h"
#include <cstring>

namespace Good
{

    UIInput& UIInput::Get()
    {
        static UIInput s_Instance;
        return s_Instance;
    }

    void UIInput::BeginFrame()
    {
        std::memcpy(m_MousePrevious, m_MouseCurrent, sizeof(m_MouseCurrent));
        std::memcpy(m_KeyPrevious,   m_KeyCurrent,   sizeof(m_KeyCurrent));

        m_ScrollX = 0.0f;
        m_ScrollY = 0.0f;
    }

    void UIInput::EndFrame()
    {
        m_InputText.clear();
    }

    void UIInput::OnMouseMove(float32 x, float32 y)
    {
        m_MouseX = x;
        m_MouseY = y;
    }

    void UIInput::OnMouseScroll(float32 dx, float32 dy)
    {
        m_ScrollX += dx;
        m_ScrollY += dy;
    }

    void UIInput::OnMouseButton(int32 btn, bool pressed)
    {
        if (btn >= 0 && btn < ButtonCount)
            m_MouseCurrent[btn] = pressed;
    }

    void UIInput::OnKey(int32 key, bool pressed)
    {
        if (key >= 0 && key < KeyCount)
            m_KeyCurrent[key] = pressed;
    }

    void UIInput::OnChar(uint32 cp)
    {
        if (cp >= 32 && cp < 127)
            m_InputText.push_back(static_cast<char>(cp));
    }

    bool UIInput::IsMouseDown(int32 btn) const
    {
        return (btn >= 0 && btn < ButtonCount) ? m_MouseCurrent[btn] : false;
    }

    bool UIInput::IsMousePressed(int32 btn) const
    {
        return (btn >= 0 && btn < ButtonCount)
            ? (m_MouseCurrent[btn] && !m_MousePrevious[btn]) : false;
    }

    bool UIInput::IsMouseReleased(int32 btn) const
    {
        return (btn >= 0 && btn < ButtonCount)
            ? (!m_MouseCurrent[btn] && m_MousePrevious[btn]) : false;
    }

    bool UIInput::IsKeyDown(int32 key) const
    {
        return (key >= 0 && key < KeyCount) ? m_KeyCurrent[key] : false;
    }

    bool UIInput::IsKeyPressed(int32 key) const
    {
        return (key >= 0 && key < KeyCount)
            ? (m_KeyCurrent[key] && !m_KeyPrevious[key]) : false;
    }

    bool UIInput::IsKeyReleased(int32 key) const
    {
        return (key >= 0 && key < KeyCount)
            ? (!m_KeyCurrent[key] && m_KeyPrevious[key]) : false;
    }

} // namespace Good