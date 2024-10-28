#pragma once
#include "../../glm/glm/vec2.hpp"

struct gravity
{
    gravity() : _gravity(glm::vec2()) {}
    glm::vec2 _gravity;
};
