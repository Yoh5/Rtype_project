/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** ControlableManager
*/

#ifndef ControlableManager_HPP_
    #define ControlableManager_HPP_
    #include "dependencies.hpp"
    #include "../../Engine/scene/scene.hpp"
    #include "controlable.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/Builtins/Components/transform.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/Builtins/Components/gravity.hpp"
    #include "../../Engine/Builtins/Components/updateTime.hpp"
    #include "../../Engine/Builtins/Components/velocity.hpp"
    #include "../../Engine/Builtins/Components/acceleration.hpp"

class ControlableManager : public BJENGINE::ASystem {
    public:
        ControlableManager(std::shared_ptr<BJENGINE::registry> reg) : BJENGINE::ASystem(reg) {}
        ~ControlableManager() {}

        void start() override
        {
            _registry->register_component<controlable>();
            _registry->register_component<updateTime>();
            _registry->register_component<property>();
        }

        void stop() override
        {
        }

        void spawnFire(size_t id)
        {
            auto newPlayerFire = _registry->spawn_entity();
            auto &transforms = _registry->get_components<transform>();
            auto &velocitys = _registry->get_components<velocity>();
            auto &properties = _registry->get_components<property>();
            glm::vec2 p_pos = transforms[id]->_position;
            glm::vec2 pos(p_pos.x + P_WIDTH, p_pos.y);
            glm::vec2 vc(velocitys[id]->_velocity.x + 60, 0);
            float h, w;
            auto d = drawable(drawableType::FIRE1);
            if (d._type == drawableType::FIRE1) {
                h = FIRE1_HEGHT;
                w = FIRE1_WIDTH;
            } else if (d._type == drawableType::FIRE2) {
                h = FIRE2_HEGHT;
                w = FIRE2_WIDTH;
            } else if (d._type == drawableType::FIRE3) {
                h = FIRE3_HEGHT;
                w = FIRE3_WIDTH;
            }
            else {
                h = FIRE4_HEGHT;
                w = FIRE4_WIDTH;
            }
            boxCollider b(0, 0, h, w);
            b.tag = "player";
            auto damage = properties[id]->damage;
            property p(1, damage);
            _registry->attach_components(newPlayerFire, transform(pos), velocity(vc), orientation(ORIENTATION::Right), acceleration(glm::vec2(1, 1)), gravity(), d, updateTime(40), b, p);
        }

        void update() override
        {
            auto &controlables = _registry->get_components<controlable>();
            auto &orientations = _registry->get_components<orientation>();
            for (auto i : _registry->entities()) {
                if (controlables.contains(i) && orientations.contains(i)
                    && controlables[i].has_value() && controlables[i].has_value()) {
                    if (controlables[i]->lastKeyType != key::NOTHING) {
                        if (controlables[i]->lastKeyType == key::UP)   {
                            orientations[i]->_orientation = ORIENTATION::Up;
                                std::cout << "[SERVER]: Move up received" << std::endl;
                        }
                        if (controlables[i]->lastKeyType == key::LEFT)  orientations[i]->_orientation = ORIENTATION::Left;
                        if (controlables[i]->lastKeyType == key::DOWN) {
                            orientations[i]->_orientation = ORIENTATION::Down;
                            std::cout << "[SERVER]: Move down received" << std::endl;
                        }
                        if (controlables[i]->lastKeyType == key::RIGHT) orientations[i]->_orientation = ORIENTATION::Right;
                        if (controlables[i]->lastKeyType == key::FIRE) {
                            std::cout << "[FIRE] id = " <<  i << std::endl;
                            spawnFire(i);
                        }
                        controlables[i]->lastKeyType = key::NOTHING;
                    } else {
                        orientations[i]->_orientation = ORIENTATION::Immobile;
                    }
                }
            }
            
        }

    protected:
    private:
};

#endif /* !ControlableManager_HPP_ */
