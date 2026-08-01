#pragma once
#include "UIWidget.h"

namespace Good
{

    class UIDropdown final : public UIWidget
    {
    public:
        static bool Draw(const std::string& label,
                         const std::vector<std::string>& items,
                         int32& selectedIndex,
                         float32 width = 160.0f);
        void Render() override;

    private:
        static std::unordered_map<uint64, bool> s_OpenState;
    };

} // namespace Good