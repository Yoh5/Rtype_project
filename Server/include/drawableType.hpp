#pragma once

enum drawableType : uint8_t {
    PLAYER_1 = 1,
    PLAYER_2 = 2,
    PLAYER_3 = 3,
    PLAYER_4 = 4,
    ENEMY_BASIC,
    ENEMY_MEDIEUM,
    ENEMY_ADVANCED,
    BOSS,
    EXPLOSION,
    FIRE1,
    FIRE2,
    FIRE3,
    FIRE4
};

struct drawable {
    drawable(drawableType t) : _type(t) {}
    drawableType _type;
};
