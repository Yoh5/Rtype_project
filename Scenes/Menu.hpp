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

namespace RTYPE
{
    class Menu
    {
    public:
        Menu(){}
        enum PAGE
        {
            HOME,
            LOGIN,
            OPTIONS,
            ROOMS,
            LOBBY
        };

        enum ASSET_TYPE
        {
            MENU_BG,
            MAIN_FONT,
            MAIN_FONT2,            
        };

        enum INPUT_TYPE
        {
            PLAY,
            OPT,
            CREATE,
            JOIN,
            LEAVE,
            READY,
            BACK_HOME,
            BACK_LOGIN,
            BACK_ROOMS,
            MUTE,
            UNMUTE,
        };

        Menu(std::shared_ptr<BJENGINE::Scene> scene, sf::RenderWindow &win, std::shared_ptr<AudioManager> audio)
        {
            _scene = scene;
            _assets =
                {
                    {MENU_BG, "assets/backgrounds/menu_bg.jpg"},
                    {MAIN_FONT, "assets/fonts/HanaleiFill-Regular.ttf"},
                    {MAIN_FONT2, "assets/fonts/LibreBaskerville-Regular.ttf"}
                };
            _audio = audio;
            init(win);
        }

        std::shared_ptr<BJENGINE::registry> registry()
        {
            return _scene->get_registry();
        }

        void operator()()
        {
            _scene->run();
        }

        void createHomePage()
        {
            int background = BJENGINE::Prefabs::UI::image(_scene->get_registry(), _assets[ASSET_TYPE::MENU_BG], glm::vec2());

            int rtype = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "R-TYPE", glm::vec2(750, 90), _main_font, glm::vec4(255, 255, 255, 255), 150);

            int playBtn = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "Play", glm::vec2(810, 400), glm::vec2(400, 150), _main_font2, glm::vec4(207, 209, 229, 0), glm::vec4(255, 255, 255, 255), glm::vec2(820, 400), 100);

            int optionBtn = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "Options", glm::vec2(740, 600), glm::vec2(400, 150), _main_font2, glm::vec4(207, 209, 229, 0), glm::vec4(255, 255, 255, 255), glm::vec2(740, 600), 100);

            _scene->get_registry()->attach_component<pressable>(playBtn, pressable());
            _scene->get_registry()->attach_component<pressable>(optionBtn, pressable());

            _input_entities[playBtn] = PLAY;
            _input_entities[optionBtn] = OPT;
            _current_page  = HOME;

        }

        void createLoginPage()
        {
            _scene->get_registry()->register_component<inputField>();

            int background_log = BJENGINE::Prefabs::UI::image(_scene->get_registry(), _assets[ASSET_TYPE::MENU_BG], glm::vec2());

            int rtype = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "R-TYPE", glm::vec2(750, 50), _main_font, glm::vec4(255, 255, 255, 255), 150);

            int usernameField = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(450, 300), glm::vec2(1000, 650), glm::vec4(35, 35, 52, 150), glm::vec4(35, 35, 52, 255));

            int usernameField1 = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(800, 320), glm::vec2(300, 70), glm::vec4(154, 162, 182, 100), glm::vec4(154, 162, 182, 255));
            int login = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Log in", glm::vec2(880, 330), _main_font, glm::vec4(255, 255, 255, 255), 50);
            
            int usernameField1_2 = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(500, 420), glm::vec2(900, 500), glm::vec4(207, 209, 229, 80), glm::vec4(154, 162, 182, 255));
            int user_name = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Name of player:", glm::vec2(550, 450), _main_font2, glm::vec4(255, 255, 255, 255), 30);
            int usernameField2 = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "Noob", glm::vec2(550, 500), glm::vec2(700, 40), _main_font2, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(550, 500), 30);

            int ip = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "IP:", glm::vec2(550, 550), _main_font2, glm::vec4(255, 255, 255, 255), 30);
            int usernameField3 = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "127.0.0.1", glm::vec2(550, 600), glm::vec2(700, 40), _main_font2, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255),glm::vec2(550, 600), 30);

            int port = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Port:", glm::vec2(550, 650), _main_font2, glm::vec4(255, 255, 255, 255), 30);
            int usernameField4 = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "8888", glm::vec2(550, 700), glm::vec2(700, 40), _main_font2, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(550, 700), 30);

            int connect_room = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "Connect", glm::vec2(850, 800), glm::vec2(200, 40), _main_font2, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(880, 800), 30);

            int back = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "back", glm::vec2(550, 860), glm::vec2(100, 50), _main_font, glm::vec4(207, 209, 229, 100), glm::vec4(255, 255, 255, 255), glm::vec2(560, 860), 30);
            

            _scene->get_registry()->attach_component<inputField>(usernameField2, inputField());
            _scene->get_registry()->attach_component<inputField>(usernameField3, inputField());
            _scene->get_registry()->attach_component<inputField>(usernameField4, inputField());
            _scene->get_registry()->attach_component<pressable>(back, pressable());


            std::vector<int> req = {
                usernameField2,
                usernameField3,
                usernameField4
            };
            _input_entities[back] = BACK_HOME;
            _scene->get_registry()->attach_component<validationButton>(connect_room, validationButton(req, "login"));
            _current_page  = LOGIN;
        }

        void createRoomsPage() {
            int background_room = BJENGINE::Prefabs::UI::image(_scene->get_registry(), _assets[ASSET_TYPE::MENU_BG], glm::vec2());

            int usernameField = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(450, 300), glm::vec2(1000, 650), glm::vec4(35, 35, 52, 150), glm::vec4(35, 35, 52, 255));

            int usernameField1 = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(800, 320), glm::vec2(300, 70), glm::vec4(154, 162, 182, 100), glm::vec4(154, 162, 182, 255));
            int rooms = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Rooms", glm::vec2(880, 330), _main_font, glm::vec4(255, 255, 255, 255), 50);       
            int usernameField2 = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(500, 420), glm::vec2(900, 350), glm::vec4(207, 209, 229, 80), glm::vec4(154, 162, 182, 255));
            
            int create = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "Create", glm::vec2(720, 800), glm::vec2(150, 40), _main_font, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(750, 800), 30);
            
            int join = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "Join", glm::vec2(1100, 800), glm::vec2(150, 40), _main_font, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(1150, 800), 30);

            int back = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "back", glm::vec2(520, 860), glm::vec2(100, 40), _main_font, glm::vec4(207, 209, 229, 100), glm::vec4(255, 255, 255, 255), glm::vec2(530, 860), 30);
            
            _scene->get_registry()->attach_component<pressable>(join, pressable());
            _scene->get_registry()->attach_component<pressable>(create, pressable());
            _scene->get_registry()->attach_component<pressable>(back, pressable());

            _current_page  = ROOMS;
            _input_entities[create] = CREATE;
            _input_entities[join] = JOIN;
            _input_entities[back] = BACK_LOGIN;
            
        }

        void createLobbyPage()
        {
            int background_lobby = BJENGINE::Prefabs::UI::image(_scene->get_registry(), _assets[ASSET_TYPE::MENU_BG], glm::vec2());

            int usernameField = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(330, 250), glm::vec2(1200, 700), glm::vec4(35, 35, 52, 150), glm::vec4(35, 35, 52, 255));
            int usernameField1 = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(800, 280), glm::vec2(280, 70), glm::vec4(154, 162, 182, 100), glm::vec4(154, 162, 182, 255));
            int lobby = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Lobby", glm::vec2(840, 285), _main_font, glm::vec4(255, 255, 255, 255), 60);       
            
            int recup_name = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Waiting for players...", glm::vec2(350, 360), _main_font2, glm::vec4(255, 255, 255, 255), 30);

            int usernameField2 = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(350, 410), glm::vec2(1150, 450), glm::vec4(207, 209, 229, 80), glm::vec4(154, 162, 182, 255));
            int chat = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Chat", glm::vec2(360, 420), _main_font, glm::vec4(255, 255, 255, 255), 30);
            int players = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Players", glm::vec2(1150, 420), _main_font, glm::vec4(255, 255, 255, 255), 30);

            int messagesBox = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(360, 480), glm::vec2(700, 300), glm::vec4(207, 209, 229, 80), glm::vec4(154, 162, 182, 255));
            int playersBox = BJENGINE::Prefabs::UI::box(_scene->get_registry(), glm::vec2(1150, 480), glm::vec2(300, 150), glm::vec4(207, 209, 229, 80), glm::vec4(154, 162, 182, 255));
            
            int message = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "", glm::vec2(360, 800), glm::vec2(550, 40), _main_font, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(360, 800), 30);
            int send = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "Send", glm::vec2(970, 800), glm::vec2(100, 40), _main_font, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(990, 800), 30);

            int ready = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "READY", glm::vec2(1210, 660), glm::vec2(160, 40), _main_font, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(1255, 665), 30);
            int leave = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "LEAVE", glm::vec2(1210, 740), glm::vec2(160, 40), _main_font, glm::vec4(4, 6, 16, 255), glm::vec4(255, 255, 255, 255), glm::vec2(1255, 745), 30);

            int back = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "back", glm::vec2(350, 900), glm::vec2(90, 40), _main_font, glm::vec4(207, 209, 229, 100), glm::vec4(255, 255, 255, 255), glm::vec2(350, 900), 30);

            _scene->get_registry()->attach_component<inputField>(message, inputField());

             std::vector<int> req = {
                message,
            };
            _scene->get_registry()->attach_component<validationButton>(send, validationButton(req, "chat"));

            _scene->get_registry()->attach_component<pressable>(ready, pressable());
            _scene->get_registry()->attach_component<pressable>(leave, pressable());
            _scene->get_registry()->attach_component<pressable>(back, pressable());

            _current_page = LOBBY;
            _input_entities[ready] = READY;
            _input_entities[back] = BACK_ROOMS;
            _input_entities[leave] = LEAVE;
        }

        void createOptionPage()
        {
            int background = BJENGINE::Prefabs::UI::image(_scene->get_registry(), _assets[ASSET_TYPE::MENU_BG], glm::vec2());


            int option = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Options", glm::vec2(200, 90), _main_font, glm::vec4(255, 255, 255, 255), 150);

            int musique = BJENGINE::Prefabs::UI::text(_scene->get_registry(), "Music", glm::vec2(200, 350), _main_font, glm::vec4(255, 255, 255, 255), 50);


            int mute = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "Mute",glm::vec2(200, 450), glm::vec2(100, 40), _main_font, glm::vec4(207, 209, 229, 0), glm::vec4(255, 255, 255, 255), glm::vec2(200, 450), 30);

            int unmute = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "Unmute", glm::vec2(200, 550), glm::vec2(100, 40), _main_font, glm::vec4(207, 209, 229, 0), glm::vec4(255, 255, 255, 255), glm::vec2(200, 550), 30);

            int back = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "back", glm::vec2(350, 900), glm::vec2(90, 40), _main_font, glm::vec4(207, 209, 229, 100), glm::vec4(255, 255, 255, 255), glm::vec2(350, 900), 30);

            _scene->get_registry()->attach_component<pressable>(back, pressable());
            _scene->get_registry()->attach_component<pressable>(mute, pressable());
            _scene->get_registry()->attach_component<pressable>(unmute, pressable());

            _input_entities[back] = BACK_HOME;
            _input_entities[mute] = MUTE;
            _input_entities[unmute] = UNMUTE;

            _current_page = OPTIONS;
        }

        //optionPage

        void instantiateRoom(int id)
        {
            int newRoom = BJENGINE::Prefabs::UI::button(_scene->get_registry(), "ROOM " + std::to_string(id), _last_room_pos , glm::vec2(800, 50), _main_font,  glm::vec4(154, 162, 182, 100), glm::vec4(255, 255, 255, 255), _last_room_pos, 30);

            _scene->get_registry()->attach_component<room>(newRoom, room(id));
            _scene->get_registry()->attach_component<pressable>(newRoom, pressable());
            _last_room_pos.y+=50;
        }

        void instantiateLobbyPlayer(std::string name)
        {
            int newPlayer = BJENGINE::Prefabs::UI::text(_scene->get_registry(), name, _last_player_pos, _main_font, glm::vec4(255, 255, 255, 255), 30);

            _scene->get_registry()->attach_component<lobbyPlayer>(newPlayer, lobbyPlayer(name));
            _last_player_pos.y+=30;
        }

        void instantiateMessage(std::string message)
        {
            BJENGINE::Prefabs::UI::text(_scene->get_registry(), message, _last_message_pos, _main_font, glm::vec4(255, 255, 255, 255), 30);

            _last_message_pos.y+=30;
        }

        void init(sf::RenderWindow &win)
        {

            auto player = _scene->get_registry()->spawn_entity();
            _scene->get_registry()->register_component<BJENGINE::inputComponent>();
            _scene->get_registry()->register_component<pressable>();

            _scene->get_registry()->register_component<inputField>();

            _scene->get_registry()->register_component<validationButton>();

            _scene->get_registry()->register_component<room>();

            _scene->get_registry()->register_component<lobbyPlayer>();

            _scene->get_registry()->attach_component<BJENGINE::inputComponent>(player, BJENGINE::inputComponent());

            _window_size = {win.getSize().x, win.getSize().y};

            _main_font = sf::Font();
            _main_font.loadFromFile(_assets[MAIN_FONT]);

            _main_font2 = sf::Font();
            _main_font2.loadFromFile(_assets[MAIN_FONT2]);

            _current_page = HOME;
            _player_idx = player;

            _last_room_pos = glm::vec2(500, 420);
            _last_player_pos =  glm::vec2(1150, 480);
            _last_message_pos = glm::vec2(360, 480);
            _game_started = false;

            createHomePage();
        }

    public:
        PAGE _current_page;
        int _player_idx;
        std::shared_ptr<BJENGINE::Scene> _scene;
        std::unordered_map<int, INPUT_TYPE> _input_entities;
        bool _game_started;
        std::shared_ptr<AudioManager> _audio;

    private:
        std::unordered_map<PAGE, std::unordered_set<int>> _pages;
        std::unordered_map<ASSET_TYPE, std::string> _assets;
        glm::vec2 _window_size;
        sf::Font _main_font;
        sf::Font _main_font2;
        glm::vec2 _last_room_pos;
        glm::vec2 _last_player_pos;
        glm::vec2 _last_message_pos;
    };

}