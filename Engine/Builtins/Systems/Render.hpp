#pragma once
#include "../../Ecs/registry.hpp"
#include <SFML/Graphics.hpp>
#include "../Components/rectangularShape.hpp"
#include "../Components/spriteRenderer.hpp"
#include "../Components/rectangularShape.hpp"
#include "../Components/circularShape.hpp"
#include "../Components/text.hpp"
#include "../Components/transform.hpp"
#include "../Components/animatedSpriteRenderer.hpp"

#define LOGGER(x) std::cout << "[BJENGINE LOGGER]: " << x << std::endl;
#define DEBUGGER(x) std::cout << "[BJENGINE DEBUGGER]: " << x << std::endl;

namespace BJENGINE
{
    class Render : public BJENGINE::ASystem
    {
    public:
        Render(std::shared_ptr<registry> r, sf::RenderWindow &win) : ASystem(r), _win_wrapper(win) {}

        void start() override
        {
            LOGGER("Rendering system launched");
            _registry->register_component<spriteRenderer>();
            _registry->register_component<rectangularShape>();
            _registry->register_component<circularShape>();
            _registry->register_component<textComponent>();
            _registry->register_component<transform>();
            _registry->register_component<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>();
        }

        void stop() override
        {
//            LOGGER("Rendering system stopped");
        }

        void update() override
        {
            auto &sprites = _registry->get_components<spriteRenderer>();
            auto &rectangles = _registry->get_components<rectangularShape>();
            auto &circles = _registry->get_components<circularShape>();
            auto &texts = _registry->get_components<textComponent>();
            auto &transforms = _registry->get_components<transform>();
            auto &_win = _win_wrapper.get();
            auto &animated = _registry->get_components<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>();

            _win.clear(sf::Color::Blue);
            for (auto i : _registry->entities())
            {
                 if (sprites.contains(i) && transforms.contains(i))
                {
                    if (sprites[i].has_value())
                    {
                        (*sprites[i])._sprite.setTexture((*sprites[i])._texture);
                        (*sprites[i])._sprite.setPosition(sf::Vector2f((*transforms[i])._position.x, (*transforms[i])._position.y));
                        _win.draw((*sprites[i])._sprite);
                    }
                }

                if (rectangles.contains(i))
                {
                    if (rectangles[i].has_value())
                    {
                        _win.draw((*rectangles[i])._rectangle);
                    }
                }

                if (circles.contains(i))
                {
                    if (circles[i].has_value())
                    {
                        _win.draw((*circles[i])._circle);
                    }
                }

                if (texts.contains(i))
                {
                    if (texts[i].has_value())
                    {
                        _win.draw((*texts[i])._text);
                    }
                }

                if (animated.contains(i) && transforms.contains(i))
                {
                    if (animated[i].has_value())
                    {
                        auto &spritesheet = (*animated[i])._states_animations[(*animated[i]).current_state];
                            if ((*animated[i])._repeat && (*animated[i])._clock.getElapsedTime().asSeconds() > (*animated[i])._delay) {
                                (*animated[i])._current_frame_idx = (((*animated[i])._current_frame_idx+1) < spritesheet.size()) ? (*animated[i])._current_frame_idx + 1 : 0;
                                (*animated[i])._clock.restart();
                            }
                            spritesheet[(*animated[i])._current_frame_idx].setPosition(sf::Vector2f((*transforms[i])._position.x, (*transforms[i])._position.y));
                        _win.draw(spritesheet[(*animated[i])._current_frame_idx]);
                    }
                }
            }
            _win.display();
        }

    private:
        void spriteAnimator() {}
        std::reference_wrapper<sf::RenderWindow> _win_wrapper;
    };
}
