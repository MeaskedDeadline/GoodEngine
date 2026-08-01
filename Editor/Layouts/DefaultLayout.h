#pragma once
#include "Layout.h"

namespace Good
{

    class DefaultLayout final : public Layout
    {
    public:
        void Apply(uint32 viewportW, uint32 viewportH) override;
        const char* GetName() const override { return "Default"; }

        static constexpr const char* HIERARCHY = "Hierarchy";
        static constexpr const char* VIEWPORT  = "Viewport";
        static constexpr const char* INSPECTOR = "Inspector";
        static constexpr const char* CONTENT   = "ContentBrowser";
        static constexpr const char* CONSOLE   = "Console";
        static constexpr const char* TOOLBAR   = "Toolbar";
        static constexpr const char* MENUBAR   = "MenuBar";
    };

} // namespace Good