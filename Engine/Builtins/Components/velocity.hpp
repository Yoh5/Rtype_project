#pragma once
#include "../../glm/glm/vec2.hpp"

struct velocity
{
    velocity(glm::vec2 velo = glm::vec2(1, 1)) : _velocity(velo) {}
    glm::vec2 _velocity;
};
