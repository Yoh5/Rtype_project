/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** infoPacket
*/

#ifndef INFOPACKET_HPP_
    #define INFOPACKET_HPP_
    #include "drawableType.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/glm/glm/vec2.hpp"

enum entityStatus : uint8_t {
    CREATE,
    UPDATE,
    DEL,
    UNCHANGE,
};

struct infoPacket {
    infoPacket(glm::vec2 p = glm::vec2(), ORIENTATION o = ORIENTATION::Immobile, drawableType t=drawableType::PLAYER_1, entityStatus s = entityStatus::UNCHANGE) : pos(p), ort(o), type(t), status(s){}
    glm::vec2 pos;
    ORIENTATION ort;
    drawableType type;
    entityStatus status;
};

#endif /* !INFOPACKET_HPP_ */
