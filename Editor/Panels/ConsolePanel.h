#pragma once
#include "Panel.h"
#include "Debug/DebugConsole.h"

namespace Good
{

    class ConsolePanel final : public Panel
    {
    public:
        ConsolePanel();

        void OnRender() override;
        const char* GetTitle() const override { return "Console"; }

        void Clear() { DebugConsole::Get().Clear(); }

    private:
        void DrawToolbar();
        void DrawEntries();
        void DrawCommandInput();

        bool m_ShowTrace{false};
        bool m_ShowInfo{true};
        bool m_ShowWarn{true};
        bool m_ShowError{true};
        bool m_AutoScroll{true};
        std::string m_CommandInput;
        float32     m_ScrollY{0.0f};
    };

} // namespace Good