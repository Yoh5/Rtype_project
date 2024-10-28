#pragma once
#include "../Components/inputField.hpp"
#include "../Components/validationButton.hpp"
#include "../Components/net.hpp"
#include "../Engine/Ecs/registry.hpp"
#include "../Engine/Builtins/Components/rectangularShape.hpp"
#include "../Engine/Builtins/Components/text.hpp"
#include "../Engine/Builtins/Components/input.hpp"
#include <SFML/Graphics.hpp>
#include "../Client/NetworkModule.hpp"
#include "../Engine/Audio/Audio.hpp"
#include "../constants/constants.hpp"

namespace RTYPE
{
    class FormSystem : public BJENGINE::ASystem
    {
    public:
        FormSystem(std::shared_ptr<BJENGINE::registry> reg, int player, std::shared_ptr<NetworkModule> netmodule,  std::shared_ptr<AudioManager> audiomodule) : BJENGINE::ASystem(reg), _player_idx(player), _audio_module(audiomodule) {}

        void start() override
        {
            _registry->register_component<inputField>();
            _registry->register_component<validationButton>();
        }

        void stop() override
        {
        }

        void update() override
        {
            auto &input = *(_registry->get_components<BJENGINE::inputComponent>())[_player_idx];

            auto &rectangles = _registry->get_components<rectangularShape>();

            auto &input_fields = _registry->get_components<inputField>();

            auto &texts = _registry->get_components<textComponent>();

            if (input._eventType == BJENGINE::INPUT_TYPE::mouseClicked) {
                _audio_module->playSound<char>();
            for (auto i : _registry->entities())
            {
                    if (rectangles.contains(i) && input_fields.contains(i) && rectangles[i].has_value() && input_fields[i].has_value())
                    {
                        for (auto j : _registry->entities())
                        {
                            if (input_fields.contains(j) && input_fields[j].has_value())
                            input_fields[i]->_editing = false;
                        }
                        if ((*rectangles[i])._rectangle.getGlobalBounds().contains(sf::Vector2f(input._click_coords.x, input._click_coords.y)))
                        {
                            input_fields[i]->_editing = true;
                        }
                    }
            }
            input.clear();
            }

            if (input._eventType == BJENGINE::INPUT_TYPE::keyPressed) {
            for (auto i : _registry->entities())
            {
                    if (rectangles.contains(i) && input_fields.contains(i) && rectangles[i].has_value() && input_fields[i].has_value())
                    {
                        if ((input_fields[i])->_editing) {
                            sf::String user_input = (*texts[i])._text.getString();
                            if (input._key_code == 8 && user_input.getSize() > 0)
                                user_input.erase(user_input.getSize() - 1);
                            else
                                user_input.insert(user_input.getSize(), input._key_code);
                            texts[i]->_text.setString((std::string)user_input);
                        }
                    }
            }
            input.clear();
            }
        }

    private:
        int _player_idx;
        std::shared_ptr<NetworkModule> _net_module;
        std::shared_ptr<AudioManager> _audio_module;
    };
}