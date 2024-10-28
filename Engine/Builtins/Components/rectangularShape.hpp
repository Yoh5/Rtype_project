#pragma once
#include <SFML/Graphics.hpp>

struct rectangularShape
{
    rectangularShape(sf::RectangleShape rect) : _rectangle(rect){}
    sf::RectangleShape _rectangle;
};