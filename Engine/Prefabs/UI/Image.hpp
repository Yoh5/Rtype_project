#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../../Builtins/Components/rectangularShape.hpp"
#include "../../Builtins/Components/text.hpp"
#include "../../Builtins/Components/spriteRenderer.hpp"
#include "../../glm/glm/vec2.hpp"
#include "../../glm/glm/vec3.hpp"

namespace BJENGINE
{
    namespace Prefabs {
        namespace UI {
            int image(std::shared_ptr<BJENGINE::registry> reg, std::string texturePath, glm::vec2 pos)
            {
                int entt = reg->spawn_entity();
                reg->register_component<spriteRenderer>();
                reg->register_component<transform>();

                reg->attach_component<spriteRenderer>(entt, spriteRenderer(texturePath));
                reg->attach_component<transform>(entt, pos);
                return entt;
            }
        }
    }
}