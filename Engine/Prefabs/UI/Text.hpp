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
            int text(std::shared_ptr<BJENGINE::registry> reg, std::string text, glm::vec2 pos, sf::Font &font, glm::vec4 textcolor, int characSize)
            {
                int entt = reg->spawn_entity();
                reg->register_component<textComponent>();

                auto txt = sf::Text();
                txt.setString(text);
                txt.setFont(font);
                txt.setFillColor(sf::Color(textcolor.x, textcolor.y, textcolor.z, textcolor.w));
                txt.setPosition({pos.x, pos.y});
                txt.setCharacterSize(characSize);
                    
                reg->attach_component<textComponent>(entt, textComponent(txt));
                return entt;
            }
        }
    }
}