#pragma once
#define P_HEIGHT 34
#define P_WIDTH 66


enum key {
    NOTHING,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FIRE,
};

struct controlable {
    controlable(key k = key::NOTHING) : lastKeyType(k) {}
    key lastKeyType;
};