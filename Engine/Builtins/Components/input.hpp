#pragma once
#include <SFML/Graphics.hpp>

namespace BJENGINE {
        enum INPUT_TYPE {
            mouseClicked,
            keyPressed,
            empty
        };
    struct inputComponent
    {
        inputComponent() : _eventType(empty), _click_coords(sf::Vector2i()), _key_code(-1) {}
        INPUT_TYPE _eventType;
        sf::Vector2i _click_coords;
        sf::Uint32 _key_code;
        void clear()
        {
            _eventType = empty;            
        }
    };
}