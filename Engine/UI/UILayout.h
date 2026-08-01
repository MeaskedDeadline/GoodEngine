#pragma once
#include "../Core/Core.h"
#include "../Math/Vec2.h"
#include "../Math/Rect.h"

namespace Good
{

    enum class UILayoutDirection : uint8 { Vertical, Horizontal };
    enum class UIAlignment       : uint8 { Start, Center, End, Stretch };

    struct UILayoutState
    {
        Vec2               cursor{0,0};
        Vec2               size{0,0};
        float32            itemSpacing{4.0f};
        UILayoutDirection  direction{UILayoutDirection::Vertical};
        UIAlignment        alignment{UIAlignment::Start};
        std::vector<float32> columnWidths;
        int32              currentColumn{0};
        float32            rowHeight{0};
        bool               sameLine{false};
    };

    class UILayout
    {
    public:
        static UILayout& Get();

        void PushLayout(const Rect& bounds, UILayoutDirection dir = UILayoutDirection::Vertical);
        void PopLayout();

        Rect NextItemRect(float32 width, float32 height);
        Vec2 GetCursor()       const;
        Vec2 GetAvailableSize() const;

        void SameLine(float32 spacing = -1.0f);
        void NewLine();
        void Indent(float32 amount = 16.0f);
        void Unindent(float32 amount = 16.0f);
        void SetNextItemWidth(float32 width);
        void Spacing(float32 amount = 8.0f);
        void Separator();

        void BeginColumns(int32 count, float32 totalWidth = 0.0f);
        void NextColumn();
        void EndColumns();

        int32 GetDepth() const { return static_cast<int32>(m_Stack.size()); }

    private:
        UILayout() = default;

        std::vector<UILayoutState> m_Stack;
        float32 m_NextItemWidth{-1.0f};
        float32 m_IndentLevel{0.0f};
    };

} // namespace Good