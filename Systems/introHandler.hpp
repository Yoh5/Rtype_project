#pragma once
#include "../Engine/Builtins/Components/animatedSpriteRenderer.hpp"
#include "../Components/intro.hpp"
#include "../Engine/Ecs/registry.hpp"

class introHandler : public BJENGINE::ASystem
{
    public:

    introHandler(std::shared_ptr<BJENGINE::registry> reg) : ASystem(reg){}

    void start() override 
    {
        _registry->register_component<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>();
        _registry->register_component<introComponent>();
    }

    void update() override
    {
        auto &animations = _registry->get_components<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>();
        auto &intro = _registry->get_components<introComponent>();

        for (auto i : _registry->entities())
        {
            if (intro.contains(i) && intro[i].has_value() && animations.contains(i) && animations[i].has_value())
            {
                if (animations[i]->_current_frame_idx >= intro[i]->_last_frame)
                {
                    intro[i]->ended=true;
                    _registry->deActivateEntity(i);
                    break;
                }
            }
        }
    }

    void stop() override{}
};
