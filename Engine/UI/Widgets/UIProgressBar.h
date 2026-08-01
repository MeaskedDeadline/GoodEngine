#pragma once
#include "UIWidget.h"

namespace Good
{

    class UIProgressBar final : public UIWidget
    {
    public:
        static void Draw(float32 value, const Vec2& size = {0,0},
                         const std::string& overlay = "");
        void Render() override;
    };

} // namespace Good