#include "GamepadCodes.h"

namespace Good
{

    const char* GamepadButtonToString(GamepadButton btn)
    {
        switch (btn)
        {
        case GamepadButton::A:           return "A";
        case GamepadButton::B:           return "B";
        case GamepadButton::X:           return "X";
        case GamepadButton::Y:           return "Y";
        case GamepadButton::LeftBumper:  return "LeftBumper";
        case GamepadButton::RightBumper: return "RightBumper";
        case GamepadButton::Back:        return "Back";
        case GamepadButton::Start:       return "Start";
        case GamepadButton::DpadUp:      return "DpadUp";
        case GamepadButton::DpadDown:    return "DpadDown";
        case GamepadButton::DpadLeft:    return "DpadLeft";
        case GamepadButton::DpadRight:   return "DpadRight";
        default:                         return "Unknown";
        }
    }

    const char* GamepadAxisToString(GamepadAxis axis)
    {
        switch (axis)
        {
        case GamepadAxis::LeftX:        return "LeftX";
        case GamepadAxis::LeftY:        return "LeftY";
        case GamepadAxis::RightX:       return "RightX";
        case GamepadAxis::RightY:       return "RightY";
        case GamepadAxis::LeftTrigger:  return "LeftTrigger";
        case GamepadAxis::RightTrigger: return "RightTrigger";
        default:                        return "Unknown";
        }
    }

} // namespace Good