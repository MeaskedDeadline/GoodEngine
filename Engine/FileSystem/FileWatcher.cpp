#include "FileWatcher.h"
#include "../Core/Log.h"
#include <chrono>

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

namespace Good
{

FileWatcher::FileWatcher()  = default;
FileWatcher::~FileWatcher() { Stop(); }

void FileWatcher::Watch(const Path& path, FileWatchCallback callback)
{
    Stop();
    m_WatchPath      = path;
    m_Callback       = std::move(callback);
    m_Watching       = true;
    m_StopRequested  = false;
    m_Thread = std::thread(&FileWatcher::WatchLoop, this);
    GOOD_LOG_INFO("FileWatcher", "Watching '{}'", path.ToString());
}

void FileWatcher::Stop()
{
    m_StopRequested = true;
    m_Watching      = false;
    if (m_Thread.joinable()) m_Thread.join();
}

void FileWatcher::WatchLoop()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    HANDLE hDir = CreateFileA(
        m_WatchPath.CStr(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr, OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS, nullptr);

    if (hDir == INVALID_HANDLE_VALUE)
    {
        GOOD_LOG_ERROR("FileWatcher", "Cannot open directory '{}'", m_WatchPath.ToString());
        m_Watching = false;
        return;
    }

    char buffer[4096];
    DWORD bytesReturned = 0;

    while (!m_StopRequested)
    {
        BOOL ok = ReadDirectoryChangesW(
            hDir, buffer, sizeof(buffer), TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
            &bytesReturned, nullptr, nullptr);

        if (!ok || m_StopRequested) break;

        FILE_NOTIFY_INFORMATION* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
        while (info)
        {
            std::wstring ws(info->FileName, info->FileNameLength / sizeof(wchar_t));
            std::string name(ws.begin(), ws.end());

            FileWatchEvent evt = FileWatchEvent::Modified;
            switch (info->Action)
            {
                case FILE_ACTION_ADDED:            evt = FileWatchEvent::Created;  break;
                case FILE_ACTION_REMOVED:          evt = FileWatchEvent::Deleted;  break;
                case FILE_ACTION_RENAMED_NEW_NAME: evt = FileWatchEvent::Renamed;  break;
                default:                           evt = FileWatchEvent::Modified; break;
            }

            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_Callback)
                m_Callback(m_WatchPath / name, evt);

            if (info->NextEntryOffset == 0) break;
            info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                reinterpret_cast<char*>(info) + info->NextEntryOffset);
        }
    }

    CloseHandle(hDir);
#else
    // Linux: заглушка — позже inotify
    while (!m_StopRequested)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
#endif

    m_Watching = false;
}

} // namespace Good