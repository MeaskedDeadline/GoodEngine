#pragma once
#include "../Core/Core.h"

namespace Good
{

    enum class MouseButton : int32
    {
        Left=0, Right=1, Middle=2, X1=3, X2=4, Count=8
    };

    const char* MouseButtonToString(MouseButton btn);

} // namespace Good