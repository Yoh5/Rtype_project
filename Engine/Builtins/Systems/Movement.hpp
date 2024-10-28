#pragma once
#include "../../Ecs/registry.hpp"
#include "../Components/transform.hpp"
#include "../Components/orientation.hpp"
#include "../Components/gravity.hpp"
#include "../Components/velocity.hpp"
#include "../Components/acceleration.hpp"
#include "../Components/updateTime.hpp"

class Movement : public BJENGINE::ASystem
{
    public:

    Movement(std::shared_ptr<BJENGINE::registry> reg) : BJENGINE::ASystem(reg)
    {

    }

    void start() override
    {
        _registry->register_component<transform>();
        _registry->register_component<velocity>();
        _registry->register_component<acceleration>();
        _registry->register_component<orientation>();
        _registry->register_component<gravity>();
    }

    void stop() override
    {

    }

    void update() override
    {
        auto &transforms = _registry->get_components<transform>();
        auto &velocities = _registry->get_components<velocity>();
        auto &accelerations = _registry->get_components<acceleration>();
        auto &directions = _registry->get_components<orientation>();
        auto &gravities = _registry->get_components<gravity>();
        auto &updateTimes = _registry->get_components<updateTime>();

        for (auto i : _registry->entities())
        {
            if 
            (
                transforms.contains(i) &&
                velocities.contains(i) &&
                accelerations.contains(i) &&
                directions.contains(i) &&
                gravities.contains(i) &&
                
                transforms[i].has_value() &&
                velocities[i].has_value() &&
                accelerations[i].has_value() &&
                directions[i].has_value() &&
                gravities[i].has_value() 
            )
            {
                if (updateTimes.contains(i) && updateTimes[i].has_value()) {
                    updateTimes[i]->end = std::chrono::system_clock::now();
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(updateTimes[i]->end - updateTimes[i]->start).count() >= updateTimes[i]->m_delay) {
                        updateTimes[i]->start = std::chrono::system_clock::now();
                        applyForces(transforms[i]->_position, gravities[i]->_gravity, velocities[i]->_velocity, accelerations[i]->_acceleration, directions[i]->_orientation);
                    }
                        
                } else {
                    applyForces(transforms[i]->_position, gravities[i]->_gravity, velocities[i]->_velocity, accelerations[i]->_acceleration, directions[i]->_orientation);
                }
            }
        }
    }
    private:
    void applyForces(glm::vec2 &pos, glm::vec2 gravity, glm::vec2 &velocity, glm::vec2 acceleration, ORIENTATION dir)
    {
        pos += gravity;
        switch (dir)
        {
         case ORIENTATION::Left:
            velocity.x -= acceleration.x;
            pos.x -= velocity.x;
            break;

            case ORIENTATION::Right:
            velocity.x += acceleration.x;
            pos.x += velocity.x;
            break;

            case ORIENTATION::Up:
            velocity.y -= acceleration.y;
            pos.y -= velocity.y;
            break;

            case ORIENTATION::Down:
            velocity.y += acceleration.y;
            pos.y += velocity.y;
            break;

            case ORIENTATION::Immobile:
            return;
        }
    }
};