#pragma once
#include "Core/Core.h"
#include <functional>

namespace Good
{

    struct UndoAction
    {
        std::string name;
        std::function<void()> undo;
        std::function<void()> redo;
    };

    class UndoSystem : public NonCopyable
    {
    public:
        static UndoSystem& Get();

        void Execute(const std::string& name,
                     std::function<void()> doAction,
                     std::function<void()> undoAction);

        void Undo();
        void Redo();

        bool CanUndo() const { return !m_UndoStack.empty(); }
        bool CanRedo() const { return !m_RedoStack.empty(); }

        const std::string& GetUndoName() const;
        const std::string& GetRedoName() const;

        void Clear();

    private:
        UndoSystem() = default;

        std::vector<UndoAction> m_UndoStack;
        std::vector<UndoAction> m_RedoStack;

        static constexpr usize MAX_UNDO = 100;
    };

} // namespace Good