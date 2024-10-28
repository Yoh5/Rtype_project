#pragma once

enum ORIENTATION : uint8_t
{
    Left,
    Right,
    Up,
    Down,
    Immobile
};

struct orientation
{
    orientation(ORIENTATION o) : _orientation(o) {}
    ORIENTATION _orientation;
};