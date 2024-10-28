#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../../Builtins/Components/rectangularShape.hpp"
#include "../../Builtins/Components/text.hpp"
#include "../../glm/glm/vec2.hpp"
#include "../../glm/glm/vec3.hpp"
#include "../../glm/glm/vec4.hpp"

namespace BJENGINE
{
    namespace Prefabs {
        namespace UI {
            int box(std::shared_ptr<BJENGINE::registry> reg, glm::vec2 pos, glm::vec2 size, glm::vec4 buttoncolor, glm::vec4 outlinecolor)
            {
                int entt = reg->spawn_entity();
                reg->register_component<rectangularShape>();

                auto box = sf::RectangleShape();
                box.setSize({size.x, size.y});
                box.setPosition({pos.x, pos.y});
                box.setFillColor(sf::Color(buttoncolor.x, buttoncolor.y, buttoncolor.z, buttoncolor.w));
                box.setOutlineColor(sf::Color(outlinecolor.x, outlinecolor.y, outlinecolor.z, outlinecolor.w));
                box.setOutlineThickness(3);
                reg->attach_component<rectangularShape>(entt, rectangularShape(box));
                return entt;
            }
        }
    }
}