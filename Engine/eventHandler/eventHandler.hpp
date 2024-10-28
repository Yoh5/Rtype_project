#pragma once
#include <functional>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <thread>
#include <iostream>

namespace BJENGINE {
    class EventHandler {
        using eventCatcher = std::function<bool(void)>;
        using callback = std::function<void(void)>;

        public:
        EventHandler()
        {
            _stop = false;
        }
    
        template<typename EVENT>
        void registerEvent(const eventCatcher &event, const callback &reply)
        {
            _events_occur[typeid(EVENT)] = event;
            _events[typeid(EVENT)] = reply;
            std::thread evtThread([this, &event] {
                watchEvents();
            });
            evtThread.join();
        }

        private:
        void watchEvents()
        {
            for (auto [type, ret] : _events_occur)
            {
                if (ret())
                {
                    _events[type]();
                }
            }
        }

        private:
        std::unordered_map<std::type_index, callback> _events;
        std::unordered_map<std::type_index, eventCatcher> _events_occur;
        bool _stop;
    };
}