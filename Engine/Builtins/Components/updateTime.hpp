#pragma once
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
using chrono = std::chrono::time_point<std::chrono::system_clock>;


struct updateTime {
    updateTime(int d) : m_delay(d), start(std::chrono::system_clock::now()), end(std::chrono::system_clock::now()) {}
    chrono start;
    chrono end;
    int m_delay;
};