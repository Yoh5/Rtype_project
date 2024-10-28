#pragma once
#include <SFML/Graphics.hpp>

struct circularShape {
    circularShape(sf::CircleShape &that) : _circle(that){}
    sf::CircleShape _circle;
};