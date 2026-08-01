#pragma once
#include "../../Engine/Core/Core.h"
#include "../../Engine/UI/UIStyle.h"

namespace Good
{

    class EditorTheme
    {
    public:
        virtual ~EditorTheme() = default;

        virtual UIStyle GetStyle() const = 0;
        virtual const char* GetName() const = 0;
    };

} // namespace Good