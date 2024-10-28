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
            int button(std::shared_ptr<registry> reg, std::string text, glm::vec2 pos, glm::vec2 size, sf::Font &font, glm::vec4 buttoncolor, glm::vec4 textcolor, glm::vec2 textpos, int textsize)
            {
                int entt = reg->spawn_entity();
                reg->register_component<textComponent>();
                reg->register_component<rectangularShape>();

                auto box = sf::RectangleShape();
                box.setSize({size.x, size.y});
                box.setPosition({pos.x, pos.y});
                box.setFillColor(sf::Color(buttoncolor.x, buttoncolor.y, buttoncolor.z, buttoncolor.w));
                auto txt = sf::Text();
                txt.setString(text);
                txt.setFont(font);
                txt.setFillColor(sf::Color(textcolor.x, textcolor.y, textcolor.z, textcolor.w));
                txt.setPosition({textpos.x, textpos.y});
                txt.setCharacterSize(textsize);
                    
                reg->attach_component<rectangularShape>(entt, rectangularShape(box));
                reg->attach_component<textComponent>(entt, textComponent(txt));
                return entt;
            }
        }
    }
}