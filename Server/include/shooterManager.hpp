
/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** shooterManager
*/

#ifndef SHOOTERMANAGER_HPP_
    #define SHOOTERMANAGER_HPP_
    #include "../../Engine/scene/scene.hpp"
    #include "../../Modules/miscellaneous.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/Builtins/Components/transform.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/Builtins/Components/gravity.hpp"
    #include "../../Engine/Builtins/Components/updateTime.hpp"
    #include "../../Engine/Builtins/Components/velocity.hpp"
    #include "../../Engine/Builtins/Components/acceleration.hpp"
    #include "shooter.hpp"
    #define FIRE1_HEGHT 35
    #define FIRE1_WIDTH 33
    #define FIRE2_HEGHT 16
    #define FIRE2_WIDTH 33
    #define FIRE3_HEGHT 32
    #define FIRE3_WIDTH 33
    #define FIRE4_HEGHT 16
    #define FIRE4_WIDTH 66


class ShooterManager : public BJENGINE::ASystem {
    public:
    ShooterManager(std::shared_ptr<BJENGINE::registry> reg) : BJENGINE::ASystem(reg) {}
    
    void start() override
    {
        _registry->register_component<shooter>();
    }

    void spawnFire(size_t id)
    {
        auto &shooters = _registry->get_components<shooter>();
        auto newFire = _registry->spawn_entity();
        auto &orientations = _registry->get_components<orientation>();
        auto &transforms = _registry->get_components<transform>();
        auto &properties = _registry->get_components<property>();
        glm::vec2 vc(shooters[id]->vx, shooters[id]->vy);
        std::cout << "[SHOOT MANAGER]: Fire" << std::endl;
        glm::vec2 pos = transforms[id]->_position;
        pos.x -= 60;
        auto t = transform(pos);
        auto v = velocity(vc);
        auto a = acceleration(glm::vec2(0, 0));
        auto g =  gravity();
        auto d = drawable(shooters[id]->fireDraw);
        auto o = orientation(orientations[id]->_orientation);
        float h, w;
        if (d._type == drawableType::FIRE1) {
            h = FIRE1_HEGHT;
            w = FIRE1_WIDTH;
        } else if (d._type == drawableType::FIRE2) {
            h = FIRE2_HEGHT;
            w = FIRE2_WIDTH;
        } else if (d._type == drawableType::FIRE3) {
            h = FIRE3_HEGHT;
            w = FIRE3_WIDTH;
        } else {
            h = FIRE4_HEGHT;
            w = FIRE4_WIDTH;
        }
        boxCollider b(0, 0, h, w);
        b.tag = "enemy";
        property p(1, properties[id]->damage);
        _registry->attach_components(newFire, t, v, a, g, d, o, updateTime(50), b, p);
    }

    void update() override
    {
        auto &shooters = _registry->get_components<shooter>();
        for (auto i : _registry->entities()) {
            if (shooters.contains(i) && shooters[i].has_value()) {
                shooters[i]->end = std::chrono::system_clock::now();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(shooters[i]->end - shooters[i]->start).count() >= shooters[i]->m_delay) {
                    spawnFire(i);
                    shooters[i]->start = std::chrono::system_clock::now();
                }
            }
        }

    }

    void stop() override
    {

    }
};


#endif /* !SHOOTERMANAGER_HPP_ */
