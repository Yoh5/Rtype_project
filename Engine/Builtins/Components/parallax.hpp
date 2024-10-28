#pragma once
#include <SFML/Graphics.hpp>

struct parallax_component
{
    parallax_component(int restartPos, float speed) : _restart_pos(restartPos), _speed(speed)
    {

    }

    int _restart_pos;
    float _speed;
};