#pragma once
#include "UIWidget.h"

namespace Good
{

    class UIScrollView
    {
    public:
        static bool Begin(const std::string& id, const Rect& rect,
                          float32& scrollY, float32 contentHeight);
        static void End();
    };

} // namespace Good