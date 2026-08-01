#include "EventSystem.h"

namespace Good
{

    EventID    EventBus::s_NextEventID    = 0;
    ListenerID EventBus::s_NextListenerID = 0;

    EventBus& EventBus::Get()
    {
        static EventBus s_Instance;
        return s_Instance;
    }

    void EventBus::Clear()
    {
        m_Listeners.clear();
    }

} // namespace Good