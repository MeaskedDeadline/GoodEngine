#include "MouseCodes.h"

namespace Good
{

    const char* MouseButtonToString(MouseButton btn)
    {
        switch (btn)
        {
        case MouseButton::Left:   return "Left";
        case MouseButton::Right:  return "Right";
        case MouseButton::Middle: return "Middle";
        case MouseButton::X1:     return "X1";
        case MouseButton::X2:     return "X2";
        default:                  return "Unknown";
        }
    }

} // namespace Good