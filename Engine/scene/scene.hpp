#pragma once
#include <unordered_set>
#include "../Ecs/registry.hpp"
#include <typeindex>
#include <typeinfo>

namespace BJENGINE
{
    class Scene
    {
        public:

        Scene() : _active(true)
        {
            _registry = std::make_shared<BJENGINE::registry>();
            _name = "default";
        } 

        Scene(std::string name) : _active(true)
        {
            _registry = std::make_shared<BJENGINE::registry>();
            _name = name;
        }        

        std::shared_ptr<BJENGINE::registry> get_registry()
        {
            return _registry;
        }

        void setActive(bool value)
        {
            _active = value;
        }
        
        std::string name() const
        {
            return _name;
        }

        void run()
        {
            if (!_active) return;
            auto tags = _registry->get_tags();
            for (auto each : tags)
            {
                _registry->run_system(each);
            }
        }

        bool isActive() const
        {
            return _active;
        }

        private:
        bool _active;
        std::shared_ptr<BJENGINE::registry> _registry;
        std::string _name;
    };
}