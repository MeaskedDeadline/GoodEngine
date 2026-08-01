#include "InputAction.h"

namespace Good
{

    InputAction::InputAction(const std::string& name) : m_Name(name) {}

    void InputAction::AddKeyBinding(Key key, InputTrigger trigger)
    {
        InputBinding b;
        b.key     = key;
        b.useKey  = true;
        b.trigger = trigger;
        m_Bindings.push_back(b);
    }

    void InputAction::AddMouseBinding(MouseButton btn, InputTrigger trigger)
    {
        InputBinding b;
        b.mouseButton = btn;
        b.useKey      = false;
        b.trigger     = trigger;
        m_Bindings.push_back(b);
    }

} // namespace Good