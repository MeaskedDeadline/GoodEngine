#pragma once
#include "UIWidget.h"

namespace Good
{

    class UISlider final : public UIWidget
    {
    public:
        static bool DrawFloat(const std::string& label, float32& value, float32 mn = 0.0f, float32 mx = 1.0f);
        static bool DrawInt(const std::string& label, int32& value, int32 mn = 0, int32 mx = 100);
        void Render() override;
    };

} // namespace Good