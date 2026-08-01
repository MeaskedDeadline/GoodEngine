#pragma once
#include "EditorTheme.h"

namespace Good
{

    class DarkTheme final : public EditorTheme
    {
    public:
        UIStyle     GetStyle() const override;
        const char* GetName()  const override { return "Dark"; }
    };

} // namespace Good