#pragma once
#include <string>
#include "transform.hpp"
#include <SFML/Graphics.hpp>

namespace BJENGINE {
    struct spriteRenderer {
        spriteRenderer(std::string texture){
            _texture =sf::Texture();
            _texture.loadFromFile(texture);
        }
        sf::Texture _texture;
        sf::Sprite _sprite;
    };
}
