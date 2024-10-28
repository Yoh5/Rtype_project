/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** shooter
*/

#ifndef SHOOTER_HPP_
    #define SHOOTER_HPP_
    #include <iostream>
    #include "drawableType.hpp"
    #include <chrono>


using chrono = std::chrono::time_point<std::chrono::system_clock>;


struct shooter {
    shooter(int d, drawableType draw, int x, int y) : m_delay(d), start(std::chrono::system_clock::now()), end(std::chrono::system_clock::now()), fireDraw(draw), vx(x), vy(y) {}
    int vx;
    int vy;
    chrono end;
    int m_delay;
    chrono start;
    drawableType fireDraw;
};

#endif /* !SHOOTER_HPP_ */
