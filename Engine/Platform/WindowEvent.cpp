#include "WindowEvent.h"

namespace Good
{

    const char* WindowEventTypeToString(WindowEventType type)
    {
        switch (type)
        {
        case WindowEventType::Close:               return "Close";
        case WindowEventType::Resize:              return "Resize";
        case WindowEventType::Focus:               return "Focus";
        case WindowEventType::LostFocus:           return "LostFocus";
        case WindowEventType::KeyPressed:          return "KeyPressed";
        case WindowEventType::KeyReleased:         return "KeyReleased";
        case WindowEventType::MouseButtonPressed:  return "MouseButtonPressed";
        case WindowEventType::MouseButtonReleased: return "MouseButtonReleased";
        case WindowEventType::MouseMoved:          return "MouseMoved";
        case WindowEventType::MouseScrolled:       return "MouseScrolled";
        case WindowEventType::CharTyped:           return "CharTyped";
        default:                                   return "None";
        }
    }

} // namespace Good