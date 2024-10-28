#pragma once
#include <unordered_map>
#include <set>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <SFML/Graphics.hpp>

namespace BJENGINE
{
    namespace BUILTINS
    {
        namespace COMPONENTS
        {
            struct animatedSpriteRenderer
            {
                animatedSpriteRenderer(std::unordered_map<std::type_index, std::vector<sf::Sprite>> anim, float delay, std::type_index curr, bool repeat=true) :
                _states_animations(anim), _clock(sf::Clock()), _delay(delay), current_state(curr), _current_frame_idx(0), _repeat(repeat) {
                }
                std::unordered_map<std::type_index, std::vector<sf::Sprite>> _states_animations;
                sf::Clock _clock;
                float _delay;
                std::type_index current_state;
                int _current_frame_idx;
                bool _repeat;
            };
        }
    }
}