#pragma once
#include "../Core/Core.h"
#include "Path.h"
#include <functional>
#include <thread>
#include <mutex>

namespace Good
{

    enum class FileWatchEvent : uint8
    {
        Created, Modified, Deleted, Renamed
    };

    using FileWatchCallback = std::function<void(const Path& path, FileWatchEvent event)>;

    class FileWatcher
    {
    public:
        FileWatcher();
        ~FileWatcher();

        void Watch(const Path& path, FileWatchCallback callback);
        void Stop();
        bool IsWatching() const { return m_Watching; }

    private:
        void WatchLoop();

        Path              m_WatchPath;
        FileWatchCallback m_Callback;
        std::thread       m_Thread;
        std::mutex        m_Mutex;
        bool              m_Watching{false};
        bool              m_StopRequested{false};
    };

} // namespace Good