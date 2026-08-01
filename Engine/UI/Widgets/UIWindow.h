#pragma once
#include "UIWidget.h"

namespace Good
{

    enum class UIWindowFlags : uint32
    {
        None        = 0,
        NoTitleBar  = 1 << 0,
        NoResize    = 1 << 1,
        NoMove      = 1 << 2,
        NoClose     = 1 << 3,
        AutoSize    = 1 << 4
    };

    inline UIWindowFlags operator|(UIWindowFlags a, UIWindowFlags b)
    {
        return static_cast<UIWindowFlags>(
            static_cast<uint32>(a) | static_cast<uint32>(b));
    }

    class UIWindow
    {
    public:
        static bool Begin(const std::string& title, bool& open,
                          Rect& rect, UIWindowFlags flags = UIWindowFlags::None);
        static void End();
    };

} // namespace Good