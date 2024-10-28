#pragma once

struct parallax_component
{
    parallax_component(int rpos, int speed)
    {
        _restart_pos =rpos;
        _speed = speed;
    }
    int _restart_pos;
    int _speed;
};