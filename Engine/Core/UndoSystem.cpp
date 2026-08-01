#include "Core/UndoSystem.h"
#include "Core/Log.h"

namespace Good
{

    static const std::string s_Empty;

    UndoSystem& UndoSystem::Get()
    {
        static UndoSystem s_Instance;
        return s_Instance;
    }

    void UndoSystem::Execute(const std::string& name,
                              std::function<void()> doAction,
                              std::function<void()> undoAction)
    {
        doAction();

        UndoAction action;
        action.name = name;
        action.undo = std::move(undoAction);
        action.redo = std::move(doAction);

        m_UndoStack.push_back(std::move(action));

        if (m_UndoStack.size() > MAX_UNDO)
            m_UndoStack.erase(m_UndoStack.begin());

        m_RedoStack.clear();
    }

    void UndoSystem::Undo()
    {
        if (m_UndoStack.empty()) return;

        auto action = std::move(m_UndoStack.back());
        m_UndoStack.pop_back();

        GOOD_LOG_INFO("Undo", "Undo: {}", action.name);
        action.undo();

        m_RedoStack.push_back(std::move(action));
    }

    void UndoSystem::Redo()
    {
        if (m_RedoStack.empty()) return;

        auto action = std::move(m_RedoStack.back());
        m_RedoStack.pop_back();

        GOOD_LOG_INFO("Undo", "Redo: {}", action.name);
        action.redo();

        m_UndoStack.push_back(std::move(action));
    }

    const std::string& UndoSystem::GetUndoName() const
    {
        return m_UndoStack.empty() ? s_Empty : m_UndoStack.back().name;
    }

    const std::string& UndoSystem::GetRedoName() const
    {
        return m_RedoStack.empty() ? s_Empty : m_RedoStack.back().name;
    }

    void UndoSystem::Clear()
    {
        m_UndoStack.clear();
        m_RedoStack.clear();
    }

} // namespace Good