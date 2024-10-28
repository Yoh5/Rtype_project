#pragma once
#include "../Engine/Builtins/Components/transform.hpp"
#include "../Components/parallax.hpp"
#include "../Engine/Ecs/registry.hpp"

class ParallaxSystem : public BJENGINE::ASystem
{
    public:
    ParallaxSystem(std::shared_ptr<BJENGINE::registry> reg) : ASystem(reg){}

    void start() override 
    {
        _registry->register_component<parallax_component>();
    }

    void update() override
    {
        auto &parallaxes = _registry->get_components<parallax_component>();
        auto &transforms = _registry->get_components<transform>();
        
        for (auto i : _registry->entities())
        {
            if (parallaxes.contains(i) && transforms.contains(i) && parallaxes[i].has_value() && transforms[i].has_value())
            {
                auto &parallax = parallaxes[i];
                auto &pos = transforms[i];
                if (pos->_position.x <= parallax->_restart_pos)
                    pos->_position.x = 0;
                pos->_position.x-=parallax->_speed;
            }
        }
    }

    void stop() override{}
};