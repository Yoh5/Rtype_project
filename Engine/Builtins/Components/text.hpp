#pragma once
#include <SFML/Graphics.hpp>

struct textComponent {
    textComponent(sf::Text &that) : _text(that){}
    sf::Text _text;
};