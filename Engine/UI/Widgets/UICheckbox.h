#pragma once
#include "UIWidget.h"

namespace Good
{

    class UICheckbox final : public UIWidget
    {
    public:
        static bool Draw(const std::string& label, bool& value);
        void Render() override;

    private:
        std::string m_Label;
        bool*       m_Value{nullptr};
    };

} // namespace Good