#pragma once
#include "UIWidget.h"

namespace Good
{

    class UIFloatField final : public UIWidget
    {
    public:
        static bool Draw(const std::string& label, float32& value,
                         float32 speed = 0.1f, float32 width = 80.0f);

        void Render() override;

    private:
        static std::unordered_map<uint64, bool>        s_EditingState;
        static std::unordered_map<uint64, std::string> s_EditingText;
        static std::unordered_map<uint64, float32>     s_DragStartValue;
        static std::unordered_map<uint64, float32>     s_DragStartMouseX;
        static std::unordered_map<uint64, float64>     s_LastClickTime;
    };

} // namespace Good