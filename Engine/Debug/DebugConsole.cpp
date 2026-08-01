#include "Debug/DebugConsole.h"
#include "Utilities/StringUtils.h"

namespace Good
{

    DebugConsole& DebugConsole::Get()
    {
        static DebugConsole s_Instance;
        return s_Instance;
    }

    void DebugConsole::Write(const LogRecord& record)
    {
        ConsoleEntry entry;
        entry.level     = record.level;
        entry.message   = record.message;
        entry.category  = record.category;
        entry.timestamp = record.timestamp;

        m_Entries.push_back(entry);

        if (m_Entries.size() > m_MaxEntries)
            m_Entries.erase(m_Entries.begin());
    }

    void DebugConsole::Clear()
    {
        m_Entries.clear();
    }

    void DebugConsole::RegisterCommand(const std::string& cmd,
        std::function<void(const std::vector<std::string>&)> handler)
    {
        m_Commands[StringUtils::ToLower(cmd)] = std::move(handler);
    }

    void DebugConsole::ExecuteCommand(const std::string& cmd)
    {
        auto parts = StringUtils::Split(StringUtils::Trim(cmd), ' ');
        if (parts.empty()) return;

        std::string name = StringUtils::ToLower(parts[0]);
        std::vector<std::string> args(parts.begin() + 1, parts.end());

        auto it = m_Commands.find(name);
        if (it != m_Commands.end())
            it->second(args);
        else
            GOOD_LOG_WARN("Console", "Unknown command: '{}'", name);
    }

} // namespace Good