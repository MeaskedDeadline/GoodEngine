#pragma once
#include "UIWidget.h"

namespace Good
{

    class UIPanel
    {
    public:
        static bool Begin(const std::string& title, const Rect& rect);
        static void End();
        static void BeginGroup(const std::string& id);
        static void EndGroup();
    };

} // namespace Good