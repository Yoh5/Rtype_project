/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** Destructor
*/

#ifndef DESTRUCTOR_HPP_
    #define DESTRUCTOR_HPP_
    #include "../../Engine/scene/scene.hpp"

class Destructor : public BJENGINE::ASystem {
    public:
        Destructor(std::shared_ptr<BJENGINE::registry> reg) : BJENGINE::ASystem(reg) {}
        ~Destructor() {}
        
        void start() override
        {
        }

        void stop() override
        {
        }

        void update() override
        {
            auto &transforms = _registry->get_components<transform>();

            for (auto i : _registry->entities()) {
                if (transforms.contains(i) && transforms[i].has_value()) {
                    auto pos = transforms[i]->_position;
                    if (pos.x > 1920 || pos.x < 0 || pos.y > 1080 || pos.y < 0) {
                        _registry->deActivateEntity(i);
                        std::cout << "[SERVER] entity deleted" << std::endl;
                    }
                }
            }
        }



    protected:
    private:
};

#endif /* !DESTRUCTOR_HPP_ */
