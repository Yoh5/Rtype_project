/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** CollisionEventManager
*/

#ifndef COLLISIONEVENTMANAGER_HPP_
    #define COLLISIONEVENTMANAGER_HPP_
    #include "property.hpp"
    #include "../../Engine/scene/scene.hpp"
    #include "../../Engine/Builtins/Components/boxCollider.hpp"


class CollisionEventManager : public BJENGINE::ASystem {
    public:
        CollisionEventManager(std::shared_ptr<BJENGINE::registry> reg) : BJENGINE::ASystem(reg)
        {
        }
        ~CollisionEventManager() {}

        void start() override
        {
        }

        void stop() override
        {
        }

        void checkCollision(size_t id1, size_t id2)
        {
            auto &properties = _registry->get_components<property>();
            if ((properties.contains(id1) && properties.contains(id2))
                    && (properties[id1].has_value() && properties[id2].has_value())) {
                    properties[id1]->health -= properties[id2]->damage;
                    properties[id2]->health -= properties[id1]->damage;
                //    std::cout << "[Collision Thrigger]: process collsion, health = " +  properties[id1]->health << ", " << properties[id2]->health << " damage = " << properties[id1]->damage  << "," << properties[id2]->damage << std::endl;
                if (properties[id1]->health <= 0 && (std::find(deleted.begin(), deleted.end(), id1) == deleted.end())) {
                        properties[id2]->score += properties[id1]->health / 20;
                //    std::cout << "[Collision Thrigger]: momo " << std::endl;
                //    std::cout << "[Collision Thrigger]: entity " << id1 << " deleted" << std::endl;
                   // _registry->deActivateEntity(id1);
                   deleted.push_back(id1);
                } if (properties[id2]->health <= 0) {
                    properties[id1]->score = properties[id2]->health / 20;
                    std::cout << "[Collision Thrigger]: toto " << std::endl;
                    std::cout << "[Collision Thrigger]: entity " << id2 << " deleted" << std::endl;
                    deleted.push_back(id2);
                }
            }
        }

        void update() override
        {
            auto &boxColliders = _registry->get_components<boxCollider>();

            for (auto i : _registry->entities()) {
                if (boxColliders.contains(i) && boxColliders[i].has_value()) {
                    for (auto id : boxColliders[i]->_collided) {
                        checkCollision(i, id);
                    }
                    boxColliders[i]->_collided.clear();
                }
            }
            for (auto del : deleted) {
           //     if (std::find(deleted.begin(), deleted.end(), del) != deleted.end())
                    _registry->deActivateEntity(del);
            }
            deleted.clear();
        }

    protected:
        std::deque<int> deleted;
    private:
};

#endif /* !COLLISIONEVENTMANAGER_HPP_ */
