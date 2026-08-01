#pragma once
#include "../Core/Core.h"

namespace Good
{

    using EventID      = uint32;
    using ListenerID   = uint32;
    using EventHandler = std::function<void(const void*)>;

    struct EventListener
    {
        ListenerID   id{0};
        EventHandler handler;
    };

    class EventBus
    {
    public:
        static EventBus& Get();

        template<typename TEvent>
        ListenerID Subscribe(std::function<void(const TEvent&)> handler)
        {
            EventID eid = GetEventID<TEvent>();
            ListenerID lid = ++s_NextListenerID;

            m_Listeners[eid].push_back({
                lid,
                [h = std::move(handler)](const void* data)
                {
                    h(*static_cast<const TEvent*>(data));
                }
            });
            return lid;
        }

        template<typename TEvent>
        void Unsubscribe(ListenerID lid)
        {
            EventID eid = GetEventID<TEvent>();
            auto it = m_Listeners.find(eid);
            if (it == m_Listeners.end()) return;

            auto& vec = it->second;
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                [lid](const EventListener& l) { return l.id == lid; }), vec.end());
        }

        template<typename TEvent>
        void Emit(const TEvent& event)
        {
            EventID eid = GetEventID<TEvent>();
            auto it = m_Listeners.find(eid);
            if (it == m_Listeners.end()) return;

            for (auto& listener : it->second)
                listener.handler(&event);
        }

        void Clear();

    private:
        EventBus() = default;

        template<typename TEvent>
        static EventID GetEventID()
        {
            static EventID s_ID = ++s_NextEventID;
            return s_ID;
        }

        std::unordered_map<EventID, std::vector<EventListener>> m_Listeners;

        static EventID    s_NextEventID;
        static ListenerID s_NextListenerID;
    };

} // namespace Good