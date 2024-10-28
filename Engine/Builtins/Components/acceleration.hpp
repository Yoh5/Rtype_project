#pragma once
#include "../../glm/glm/vec2.hpp"

struct acceleration
{
    acceleration(glm::vec2 acc = glm::vec2()) : _acceleration(acc) {}
    glm::vec2 _acceleration;
};
