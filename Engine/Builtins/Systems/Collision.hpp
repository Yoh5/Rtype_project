#pragma once
#include "../../Ecs/registry.hpp"
#include "../Components/rectangularShape.hpp"
#include "../Components/spriteRenderer.hpp"
#include "../Components/rectangularShape.hpp"
#include "../Components/circularShape.hpp"
#include "../Components/text.hpp"
#include "../Components/transform.hpp"
#include "../Components/input.hpp"
#include "../Components/boxCollider.hpp"

class CollisionSystem : public BJENGINE::ASystem
{
    public:
    CollisionSystem(std::shared_ptr<BJENGINE::registry> reg) : BJENGINE::ASystem(reg)
    {
    }

    void start() override
    {
        _registry->register_component<boxCollider>();
    }

    void stop() override
    {
    }

    void update() override
    {
        auto &colliders = _registry->get_components<boxCollider>();
        auto &transforms = _registry->get_components<transform>();

        for (auto i : _registry->entities())
        {
            for (auto j : _registry->entities())
            {
                if ((i != j) && colliders.contains(i) && transforms.contains(i) && transforms.contains(j) && colliders.contains(j) && colliders[i].has_value() &&
                        colliders[j].has_value() && transforms[i].has_value() && transforms[j].has_value() && collides(colliders[i]->_top, colliders[i]->_left, colliders[i]->_right, colliders[i]->_bottom, colliders[j]->_top, colliders[j]->_left, colliders[j]->_right, colliders[j]->_bottom,
                    transforms[i]->_position.x, transforms[i]->_position.y, transforms[j]->_position.x, transforms[j]->_position.y))
                {
                    if (colliders[i]->tag != colliders[j]->tag && std::find(colliders[j]->_collided.begin(), colliders[j]->_collided.end(), i) == colliders[j]->_collided.end())
                    {
                        colliders[i]->_collided.push_back(j);
                    }
                }
            }
        }
    }

    bool collides(float top, float left, float right, float bottom,
                  float top2, float left2, float right2, float bottom2,
                  float posX, float posY, float posX2, float posY2)
    {
        left += posX;
        right += posX;
        top += posY;
        bottom += posY;

        left2 += posX2;
        right2 += posX2;
        top2 += posY2;
        bottom2 += posY2;

        if (right < left2 || left > right2 || bottom < top2 || top > bottom2)
        {
            return false;
        }

        return true;
    }
};