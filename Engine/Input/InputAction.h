#pragma once
#include "../Core/Core.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Good
{

    enum class InputTrigger : uint8
    {
        Pressed = 0,
        Released,
        Held
    };

    struct InputBinding
    {
        Key          key{Key::Unknown};
        MouseButton  mouseButton{MouseButton::Left};
        bool         useKey{true};
        InputTrigger trigger{InputTrigger::Pressed};
    };

    class InputAction
    {
    public:
        InputAction() = default;
        explicit InputAction(const std::string& name);

        void AddKeyBinding(Key key, InputTrigger trigger = InputTrigger::Pressed);
        void AddMouseBinding(MouseButton btn, InputTrigger trigger = InputTrigger::Pressed);

        const std::string& GetName() const { return m_Name; }
        const std::vector<InputBinding>& GetBindings() const { return m_Bindings; }

    private:
        std::string              m_Name;
        std::vector<InputBinding> m_Bindings;
    };

} // namespace Good