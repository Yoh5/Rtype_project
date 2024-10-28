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
#include "../Components/parallax.hpp"
#include "../Engine/Audio/Audio.hpp"

class Game
{
    public:

    enum drawable : uint8_t{
        PLAYER_1 = 1,
        PLAYER_2 = 2,
        PLAYER_3 = 3,
        PLAYER_4 = 4,
        ENEMY_BASIC,
        ENEMY_MEDIUM,
        ENEMY_ADVANCED,
        BOSS,
        EXPLOSION,
        FIRE1,
        FIRE2,
        FIRE3,
        FIRE4,
    };

    Game(BJENGINE::Scene &scene, std::shared_ptr<AudioManager> audio) : _scene(scene), _audio(audio)
    {
        init();
    }
    ~Game()
    {
        _audio->stopMusic<float>();
    }

    void init()
    {
        _scene.get_registry()->register_component<BJENGINE::inputComponent>();
        _scene.get_registry()->register_component<textComponent>();
        _scene.get_registry()->register_component<parallax_component>();

            _main_font = sf::Font();
            _main_font.loadFromFile("assets/fonts/HanaleiFill-Regular.ttf");
        _paths = {
            {PLAYER_1, {"assets/Game/player_1_001.png", "assets/Game/player_1_002.png", "assets/Game/player_1_003.png"}},

            {PLAYER_2, {"assets/Game/player_2_001.png", "assets/Game/player_2_003.png", "assets/Game/player_2_004.png"}},

            {PLAYER_3, {"assets/Game/player_3_001.png", "assets/Game/player_3_002.png", "assets/Game/player_3_003.png", "assets/Game/player_3_004.png"}},

            {PLAYER_4, {"assets/Game/player_4_001.png", "assets/Game/player_4_002.png", "assets/Game/player_4_003.png", "assets/Game/player_4_004.png"}},
             {ENEMY_BASIC, {"assets/Game/nmie_1_002.png", "assets/Game/nmie_1_003.png", "assets/Game/nmie_1_004.png", "assets/Game/nmie_1_005.png", "assets/Game/nmie_1_006.png"}},

             {ENEMY_MEDIUM, {"assets/Game/nmie_4_005.png", "assets/Game/nmie_4_006.png", "assets/Game/nmie_4_007.png", "assets/Game/nmie_4_008.png", "assets/Game/nmie_4_009.png"}},

             {ENEMY_ADVANCED, {"assets/Game/nmie_5_001.png", "assets/Game/nmie_5_002.png"}},

             {BOSS, {"assets/Game/nmie_2_003.png", "assets/Game/nmie_2_004.png"}},

            {EXPLOSION, {"assets/Game/explosion1.png", "assets/Game/explosion2.png", "assets/Game/explosion3.png", "assets/Game/explosion4.png", "assets/Game/explosion5.png", "assets/Game/explosion6.png"}},

            {FIRE1, {"assets/Game/Beam_1_002.png"}},

            {FIRE2, {"assets/Game/Beam_3_002.png"}},

            {FIRE3, {"assets/Game/Beam_3_007.png"}},

            {FIRE4, {"assets/Game/Beam_4_003.png"}}


        };

        for (auto [type, paths] : _paths)
        {
            int i = 0;
            for (auto path : paths)
            {
                _textures[type].push_back(std::make_shared<sf::Texture>());
                _sprites[type].push_back(sf::Sprite());
                _textures[type][i]->loadFromFile(path);
                _sprites[type][i].setTexture(*_textures[type][i].get());
                i++;
            }
        }

        _player_idx = _scene.get_registry()->spawn_entity();
        int background = BJENGINE::Prefabs::UI::image(_scene.get_registry(), "assets/Game/background.png", glm::vec2());
 
        int score = BJENGINE::Prefabs::UI::text(_scene.get_registry(), "Score:", glm::vec2(), _main_font, glm::vec4(255, 255, 255, 255), 30);       
        _score= BJENGINE::Prefabs::UI::text(_scene.get_registry(), "0", glm::vec2(100, 0), _main_font, glm::vec4(255, 255, 255, 255), 30);       
 
        int pv = BJENGINE::Prefabs::UI::text(_scene.get_registry(), "PV:", glm::vec2(200, 0), _main_font, glm::vec4(255, 255, 255, 255), 30);       
        _hp = BJENGINE::Prefabs::UI::text(_scene.get_registry(), "0: ", glm::vec2(300, 0), _main_font, glm::vec4(255, 255, 255, 255), 30);       
 
        int rank = BJENGINE::Prefabs::UI::text(_scene.get_registry(), "LEVEL:", glm::vec2(400, 0), _main_font, glm::vec4(255, 255, 255, 255), 30);       

        _level = BJENGINE::Prefabs::UI::text(_scene.get_registry(), "1", glm::vec2(500, 0), _main_font, glm::vec4(255, 255, 255, 255), 30);       

        _scene.get_registry()->attach_component<parallax_component>(background, parallax_component(-1920, 3));

        _scene.get_registry()->register_component<BJENGINE::inputComponent>();

        _scene.get_registry()->attach_component<BJENGINE::inputComponent>(_player_idx, BJENGINE::inputComponent());

    }

    void operator()()
    {
        _scene.run();
    }

        void createLosePage() 
        {
        int background = BJENGINE::Prefabs::UI::image(_scene.get_registry(), "assets/Game/background.png", glm::vec2());

            int usernameField = BJENGINE::Prefabs::UI::box(_scene.get_registry(), glm::vec2(450, 300), glm::vec2(1000, 650), glm::vec4(35, 35, 52, 150), glm::vec4(35, 35, 52, 255));
            
            int exit = BJENGINE::Prefabs::UI::button(_scene.get_registry(), "exit", glm::vec2(1390, 300), glm::vec2(60, 40), _main_font, glm::vec4(207, 209, 229, 100), glm::vec4(255, 255, 255, 255), glm::vec2(1390, 300), 30);

            int game_over = BJENGINE::Prefabs::UI::text(_scene.get_registry(), "Game over", glm::vec2(580, 330), _main_font, glm::vec4(255, 255, 255, 255), 150);       
            
            int usernameField1 = BJENGINE::Prefabs::UI::box(_scene.get_registry(), glm::vec2(800, 600), glm::vec2(320, 100), glm::vec4(207, 209, 229, 80), glm::vec4(154, 162, 182, 255));
            int win = BJENGINE::Prefabs::UI::text(_scene.get_registry(), "You win", glm::vec2(820, 605), _main_font, glm::vec4(255, 255, 255, 255), 80);       
            
            int usernameField2 = BJENGINE::Prefabs::UI::box(_scene.get_registry(), glm::vec2(800, 800), glm::vec2(335, 100), glm::vec4(154, 162, 182, 100), glm::vec4(154, 162, 182, 255));
            

            

        }

        void createWinPage() 
        {
        int background = BJENGINE::Prefabs::UI::image(_scene.get_registry(), "assets/Game/background.png", glm::vec2());

            int usernameField = BJENGINE::Prefabs::UI::box(_scene.get_registry(), glm::vec2(450, 300), glm::vec2(1000, 650), glm::vec4(35, 35, 52, 150), glm::vec4(35, 35, 52, 255));
            
            int exit = BJENGINE::Prefabs::UI::button(_scene.get_registry(), "exit", glm::vec2(1390, 300), glm::vec2(60, 40), _main_font, glm::vec4(207, 209, 229, 100), glm::vec4(255, 255, 255, 255), glm::vec2(1390, 300), 30);

            int game_over = BJENGINE::Prefabs::UI::text(_scene.get_registry(), "Game over", glm::vec2(580, 330), _main_font, glm::vec4(255, 255, 255, 255), 150);       
            
            int usernameField1 = BJENGINE::Prefabs::UI::box(_scene.get_registry(), glm::vec2(800, 600), glm::vec2(350, 100), glm::vec4(207, 209, 229, 80), glm::vec4(154, 162, 182, 255));
            int win = BJENGINE::Prefabs::UI::text(_scene.get_registry(), "You lose", glm::vec2(820, 605), _main_font, glm::vec4(255, 255, 255, 255), 80);       
            
            

        }

    std::shared_ptr<BJENGINE::registry> registry()
    {
        return _scene.get_registry();
    }

    public:

    BJENGINE::Scene _scene;
    int _player_idx;
    std::unordered_map<drawable, std::vector<sf::Sprite>> _sprites;
    std::unordered_map<drawable, std::vector<std::shared_ptr<sf::Texture>>> _textures;
    std::unordered_map<drawable, std::vector<std::string>> _paths;
    int _hp;
    int _score;
    int _level;

    std::shared_ptr<AudioManager> _audio;

    private:
    sf::Font _main_font;

};