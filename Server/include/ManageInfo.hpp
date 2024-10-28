/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** ManageInfo
*/

#ifndef MANAGEINFO_HPP_
    #define MANAGEINFO_HPP_
    #include "../../Engine/scene/scene.hpp"
    #include "infoPacket.hpp"
    #include "../../Modules/miscellaneous.hpp"
// Should be the last system to be created

class ManageInfo : public BJENGINE::ASystem {
    public:
        ManageInfo(std::shared_ptr<BJENGINE::registry> reg, std::shared_ptr<std::vector<infoPacket>> e) : BJENGINE::ASystem(reg), entities(e) {}
        ~ManageInfo() {}

        void start() override
        {
            _registry->register_component<drawable>();
            _registry->register_component<transform>();
            _registry->register_component<orientation>();
            _registry->register_component<infoPacket>();
         }

        void stop() override
        {
        }

        void update() override
        {
   
        }

    protected:
        std::shared_ptr<std::vector<infoPacket>> entities; 
    private:
};

#endif /* !MANAGEINFO_HPP_ */
