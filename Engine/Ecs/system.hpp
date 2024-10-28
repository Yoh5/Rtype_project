#pragma once
#include "registry.hpp"

namespace BJENGINE {
    class registry;
    class System {
        public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void update() = 0;
    };

    class ASystem : public System
    {
        public:
        ASystem(std::shared_ptr<BJENGINE::registry> r) : _registry(r)
        {
        }

        virtual void start() = 0;

        virtual void stop() = 0;

        virtual void update() = 0;

        protected:
        std::shared_ptr<BJENGINE::registry> _registry;
    };
}
