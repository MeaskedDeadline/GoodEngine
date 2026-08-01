#pragma once
#include "Core/Core.h"
#include "Core/Log.h"

namespace Good
{

    struct ConsoleEntry
    {
        LogLevel    level{LogLevel::Info};
        std::string message;
        std::string category;
        int64       timestamp{0};
    };

    class DebugConsole : public ILogSink, public NonCopyable
    {
    public:
        static DebugConsole& Get();

        void Write(const LogRecord& record) override;
        void Flush() override {}

        void Clear();
        void SetMaxEntries(uint32 max) { m_MaxEntries = max; }

        const std::vector<ConsoleEntry>& GetEntries() const { return m_Entries; }
        uint32 GetEntryCount() const { return static_cast<uint32>(m_Entries.size()); }

        void ExecuteCommand(const std::string& cmd);
        void RegisterCommand(const std::string& cmd,
                             std::function<void(const std::vector<std::string>&)> handler);

    private:
        DebugConsole() = default;

        std::vector<ConsoleEntry> m_Entries;
        uint32 m_MaxEntries{1000};
        std::unordered_map<std::string,
            std::function<void(const std::vector<std::string>&)>> m_Commands;
    };

} // namespace Good