#pragma once
#include "../Engine/scene/scene.hpp"
#include "../Engine/Prefabs/UI/Image.hpp"
#include "../Engine/Prefabs/UI/Button.hpp"
#include "../Engine/Prefabs/UI/Box.hpp"
#include "../Engine/Prefabs/UI/Text.hpp"
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>
#include "../Engine/Builtins/Components/rectangularShape.hpp"
#include "../Engine/Builtins/Components/spriteRenderer.hpp"
#include "../Engine/Builtins/Components/input.hpp"
#include "../Engine/Builtins/Systems/Render.hpp"
#include "../Engine/Builtins/Systems/Input.hpp"
#include "../Components/pressable.hpp"
#include "../Components/inputField.hpp"
#include "../Components/validationButton.hpp"
#include "../Components/room.hpp"
#include "../Components/intro.hpp"
#include "../Engine/Audio/Audio.hpp"
#include "../Components/lobbyPlayer.hpp"

class IntroScene
{
public:
    IntroScene(BJENGINE::Scene &sc, std::shared_ptr<AudioManager> audio) : _scene(sc), _audio(audio)
    {
        init();
    }

    void operator()()
    {
        _scene.run();
        watchIntro();
    }

    void init()
    {
        _ended = false;
        _scene.get_registry()->register_component<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>();
        _scene.get_registry()->register_component<transform>();
        _scene.get_registry()->register_component<pressable>();
        _scene.get_registry()->register_component<BJENGINE::inputComponent>();

        _player_idx = _scene.get_registry()->spawn_entity();

        _scene.get_registry()->attach_component<BJENGINE::inputComponent>(_player_idx, BJENGINE::inputComponent());

            _main_font = sf::Font();
            _main_font.loadFromFile("assets/fonts/HanaleiFill-Regular.ttf");

        _intro_entt = _scene.get_registry()->spawn_entity();
        std::string ref = "assets/Intro/";

        for (int i = 0; i < 301; i++)
        {
            auto path = (i == 300) ? ref + (std::to_string(i) + ".png") : ref + (std::to_string(i+1) + ".png");
            _intro_textures.push_back(std::make_shared<sf::Texture>());
            if (!_intro_textures[i]->loadFromFile(path))
                throw BJENGINE::Error("intro init", "62", "Failed to Load Texture");
            _intro_sprites.push_back(sf::Sprite(*_intro_textures[i]));
            _intro_sprites[i].setScale(sf::Vector2f(3, 3));
        }

        struct intro
        {
        };

        std::unordered_map<std::type_index, std::vector<sf::Sprite>> frames = {{std::type_index(typeid(std::vector<int>)), _intro_sprites}};

        _scene.get_registry()->attach_component<transform>(_intro_entt, transform());
        _scene.get_registry()->attach_component<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>(_intro_entt, BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer(frames, 0.1f, std::type_index(typeid(std::vector<int>))));
        createSkip();
        _last_frame = 299;
        _audio->playMusic<bool>(true);
    }

    void createSkip()
    {
        _skip_btn = BJENGINE::Prefabs::UI::button(_scene.get_registry(), "SKIP", glm::vec2(), glm::vec2(100, 40), _main_font, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(), 30);        
        _scene.get_registry()->attach_component(_skip_btn, pressable());
    }


    void watchIntro()
    {
        auto &intro_cmpt = _scene.get_registry()->get_components<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>()[_intro_entt];
        auto &input_cmpt =_scene.get_registry()->get_components<BJENGINE::inputComponent>()[_player_idx];
        auto &skip_cmpt =_scene.get_registry()->get_components<rectangularShape>()[_skip_btn];

        if 
        (
            (intro_cmpt->_current_frame_idx >= _last_frame) 
            ||  
            (
                input_cmpt->_eventType == BJENGINE::INPUT_TYPE::mouseClicked && 
                skip_cmpt->_rectangle.getGlobalBounds().contains(sf::Vector2f(input_cmpt->_click_coords.x, input_cmpt->_click_coords.y))
            )
        ) {
            _ended  = true;
            _audio->stopMusic<bool>();
        }
    }

    std::shared_ptr<BJENGINE::registry> registry()
    {
        return _scene.get_registry();
    }

    bool _ended;
    std::shared_ptr<AudioManager> _audio;
    BJENGINE::Scene _scene;
    int _player_idx;

private:
    std::vector<std::shared_ptr<sf::Texture>> _intro_textures;
    std::vector<sf::Sprite> _intro_sprites;
    sf::Font _main_font;
    int _skip_btn;
    int _intro_entt;
    int _last_frame;
};
