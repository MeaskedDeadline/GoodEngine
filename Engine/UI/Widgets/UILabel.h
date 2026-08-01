#pragma once
#include "UIWidget.h"

namespace Good
{

    class UILabel final : public UIWidget
    {
    public:
        static bool Draw(const std::string& text, const Vec2& pos = {0,0});
        static bool DrawColored(const std::string& text, const Color& color, const Vec2& pos = {0,0});

        void SetText(const std::string& t) { m_Text = t; }
        void Render() override;

    private:
        std::string m_Text;
    };

} // namespace Good