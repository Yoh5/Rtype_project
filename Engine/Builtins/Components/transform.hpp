#pragma once
#include "../../glm/glm/vec2.hpp"

struct transform {
    transform(glm::vec2 pos = glm::vec2(), glm::vec2 rotation = glm::vec2(), glm::vec2 scale = glm::vec2()) : _position(pos), _rotation(rotation), _scale(scale) {}
    glm::vec2 _position;
    glm::vec2 _rotation;
    glm::vec2 _scale;
};