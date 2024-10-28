#pragma once
#include "../../Ecs/registry.hpp"
#include <SFML/Graphics.hpp>
#include "../Components/rectangularShape.hpp"
#include "../Components/spriteRenderer.hpp"
#include "../Components/rectangularShape.hpp"
#include "../Components/circularShape.hpp"
#include "../Components/text.hpp"
#include "../Components/transform.hpp"
#include "../Components/input.hpp"

namespace BJENGINE 
{
    namespace ECS
    {
        namespace BUILTINS
        {
            namespace SYSTEMS
            {
                class Input : public BJENGINE::ASystem
                {
                    public:
                    
                    Input(std::shared_ptr<BJENGINE::registry> reg, sf::RenderWindow & win, int player) : ASystem(reg), _win_wrapper(win), player_idx(player){}

                    void start() override
                    {
                        
                    }

                    void update() override
                    {
                        sf::Event evt;
                        auto &win = _win_wrapper.get();
                        auto &inputs = _registry->get_components<inputComponent>();
                        while (win.pollEvent(evt)) {
                            auto mousePos = sf::Mouse::getPosition(win);
                                switch(evt.type) {
                                    case (sf::Event::Closed):
                                        win.close();
                                        break;
                                    case( sf::Event::MouseButtonPressed):
                                        (*inputs[player_idx])._eventType = INPUT_TYPE::mouseClicked;
                                        (*inputs[player_idx])._click_coords = {mousePos.x, mousePos.y};
                                        break;
                                    case (sf::Event::TextEntered):
                                        (*inputs[player_idx])._eventType = INPUT_TYPE::keyPressed; 
                                        (*inputs[player_idx])._key_code = evt.text.unicode;
                                        break;
                            }
                        }
                    }

                    void stop() override
                    {

                    }

                    private:
                    std::reference_wrapper<sf::RenderWindow> _win_wrapper;
                    int player_idx;
                };
            }
        }
    }
}