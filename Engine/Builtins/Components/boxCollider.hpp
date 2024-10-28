#pragma once
#include <string>
#include <deque>
#include <functional>

struct boxCollider
{
    boxCollider(float t, float l, float b, float r) : _left(l), _top(t), _right(r), _bottom(b) {}
    float _top, _left, _bottom, _right;
    std::string tag;
    std::deque<int> _collided;
};
