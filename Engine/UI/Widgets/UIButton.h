#pragma once
#include "UIWidget.h"

namespace Good
{

    class UIButton final : public UIWidget
    {
    public:
        static bool Draw(const std::string& label, const Vec2& size = {0,0});
        static bool DrawIcon(const std::string& icon, const std::string& tooltip = "");

        void SetLabel(const std::string& l) { m_Label = l; }
        void Render() override;

    private:
        std::string m_Label{"Button"};
    };

} // namespace Good