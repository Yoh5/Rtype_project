/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** property
*/

#ifndef PROPERTY_HPP_
    #define PROPERTY_HPP_
    #include <iostream>
    #include "infoPacket.hpp"

struct property {
    property(int h, int d) : health(h), damage(d), score(0) {}
    int health;
    int damage;
    int score;
};

struct infoStats {
    infoStats(int h = 0, int s = 0, entityStatus t = entityStatus::UNCHANGE, int p = 1) : health(h), score(s), status(t), player(p) {}
    int health;
    int score;
    entityStatus status;
    int player;
};

#endif /* !PROPERTY_HPP_ */
