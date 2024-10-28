/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** CollisionTrigger
*/

#ifndef COLLISIONTRIGGER_HPP_
    #define COLLISIONTRIGGER_HPP_
    #include "../../Engine/scene/scene.hpp"

void enemyCollisionTrigger(size_t id1, size_t id2, std::shared_ptr<BJENGINE::registry> reg)
{
    auto properties = reg->get_components<property>();

}

void playerCollisionTrigger(size_t id1, size_t id2, std::shared_ptr<BJENGINE::registry> reg)
{
}


#endif /* !COLLISIONTRIGGER_HPP_ */
