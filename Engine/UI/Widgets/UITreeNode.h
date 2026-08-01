#pragma once
#include "UIWidget.h"

namespace Good
{

    class UITreeNode
    {
    public:
        static bool Begin(const std::string& label,
                          bool defaultOpen = false,
                          bool* selected   = nullptr);
        static void End();
        static bool BeginLeaf(const std::string& label, bool* selected = nullptr);
    };

} // namespace Good