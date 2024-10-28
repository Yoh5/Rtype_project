#pragma once
#include "../Engine/Builtins/Components/rectangularShape.hpp"
#include "../Components/parallax.hpp"
#include "../Engine/Ecs/registry.hpp"
#include <SFML/Graphics.hpp>

class onHoverSystem : public BJENGINE::ASystem
{
public:
    onHoverSystem(std::shared_ptr<BJENGINE::registry> reg, sf::RenderWindow &win) : ASystem(reg) , _win(win) {}

    void start() override
    {
        _registry->register_component<parallax_component>();
    }

    void update() override
    {
        auto &rectangles = _registry->get_components<rectangularShape>();
        auto mousePos = sf::Mouse::getPosition(_win);

        for (auto i : _registry->entities())
        {
            if (rectangles.contains(i) && rectangles[i].has_value())
            {
                if ((*rectangles[i])._rectangle.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y)))
                {
                    rectangles[i]->_rectangle.setOutlineThickness(3);
                    rectangles[i]->_rectangle.setOutlineColor(sf::Color::White);
                }
                else
                    rectangles[i]->_rectangle.setOutlineColor(sf::Color::Transparent);
            }
        }
    }

    void stop() override {}

    private:
    std::reference_wrapper<sf::RenderWindow> _win;
};