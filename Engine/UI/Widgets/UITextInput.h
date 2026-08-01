#pragma once
#include "UIWidget.h"

namespace Good
{

    class UITextInput final : public UIWidget
    {
    public:
        // Возвращает true если Enter был нажат
        static bool DrawWithEnter(const std::string& label, std::string& value,
                                  const std::string& hint = "", float32 width = 200.0f,
                                  bool* outChanged = nullptr);

        static bool Draw(const std::string& label, std::string& value,
                         const std::string& hint = "", float32 width = 200.0f);

        void Render() override;

    private:
        // Курсор для каждого поля (по адресу переменной)
        static std::unordered_map<uint64, usize> s_CursorPositions;
        // Таймер мигания курсора
        static float32 s_BlinkTimer;

        static usize& GetCursor(uint64 id, usize defaultPos);
    };

} // namespace Good