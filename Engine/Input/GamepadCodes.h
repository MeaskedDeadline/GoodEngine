#pragma once
#include "../Core/Core.h"

namespace Good
{

    enum class GamepadButton : int32
    {
        A=0, B, X, Y,
        LeftBumper, RightBumper,
        Back, Start, Guide,
        LeftThumb, RightThumb,
        DpadUp, DpadRight, DpadDown, DpadLeft,
        Count
    };

    enum class GamepadAxis : int32
    {
        LeftX=0, LeftY,
        RightX, RightY,
        LeftTrigger, RightTrigger,
        Count
    };

    const char* GamepadButtonToString(GamepadButton btn);
    const char* GamepadAxisToString(GamepadAxis axis);

} // namespace Good