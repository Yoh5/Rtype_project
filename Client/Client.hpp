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
#include "../Engine/Builtins/Systems/Collision.hpp"
#include "../Engine/Builtins/Systems/Input.hpp"
#include "../Engine/Builtins/Systems/Collision.hpp"
#include "../Engine/Builtins/Systems/Movement.hpp"
#include "../Scenes/Menu.hpp"
#include "../Scenes/Intro.hpp"
#include "../Scenes/Game.hpp"
#include "../Systems/PageSwitchSystem.hpp"
#include "../Systems/FormSystem.hpp"
#include "../Systems/GameUpdateSystem.hpp"
#include "../Systems/onHover.hpp"
#include "NetworkModule.hpp"
#include "../Engine/Audio/Audio.hpp"
#include "../constants/constants.hpp"
#include "../Systems/ParallaxSystem.hpp"
#include "../Systems/introHandler.hpp"


namespace RTYPE
{

    class Client
    {
    public:
        Client(int port) : _port(port)
        {
            init();
        }

        void init()
        {
            _win.create(sf::VideoMode::getDesktopMode(), "RTYPE");
            _win.setFramerateLimit(60);
        }

        void run()
        {
            struct RENDER_TAG
            {
            };
            struct INPUT_TAG
            {
            };
            struct PAGE_SWITCH_TAG
            {
            };
            struct FORM_TAG
            {
            };

            struct CLICK_SOUND
            {
            };

            struct MENU_MUSIC
            {
            };

            std::unordered_map<std::type_index, std::string> sounds =
                {
                    {typeid(char), "assets/sounds/click.wav"},
                    {typeid(bool), "assets/sounds/shot.wav"},
                    {typeid(std::string), "assets/sounds/boom.wav"},
                    {typeid(double), "assets/sounds/defeat.ogg"},
                    {typeid(float), "assets/sounds/victory.ogg"},
                    {typeid(int), "assets/sounds/logged.ogg"}};


            std::unordered_map<std::type_index, std::string> musics =
                {
                    {typeid(std::string), "assets/musiques/menu.ogg"},
                    {typeid(float), "assets/musiques/game_song.ogg"},
                    {typeid(bool), "assets/musiques/cinematique.ogg"},
                    };

            std::shared_ptr<BJENGINE::Scene> menu_scene = std::make_shared<BJENGINE::Scene>("menu");


            auto net_module = std::make_shared<NetworkModule>(_port);


            auto audio_module = std::make_shared<AudioManager>(sounds, musics);

            Menu menu_handler(menu_scene, _win, audio_module);


            auto _reg = menu_handler.registry();
            _reg->add_system<int>(std::make_shared<BJENGINE::ECS::BUILTINS::SYSTEMS::Input>(_reg, _win, menu_handler._player_idx));
            _reg->add_system<std::tuple<int>>(std::make_shared<onHoverSystem>(_reg, _win));
            _reg->add_system<float>(std::make_shared<RTYPE::PageSwitchSystem>(_reg, menu_handler, net_module, audio_module));
            _reg->add_system<double>(std::make_shared<RTYPE::FormSystem>(_reg, menu_handler._player_idx, net_module, audio_module));
            _reg->add_system<bool>(std::make_shared<BJENGINE::Render>(_reg, _win));
            _reg->add_system<char>(std::make_shared<introHandler>(_reg));

            _reg->init_systems();

            auto &input= _reg->get_components<net>()[menu_handler._player_idx];


//******************************
            BJENGINE::Scene game_scene("game");
            Game game_handler(game_scene, audio_module);

            auto _greg = game_handler.registry();

            BJENGINE::Scene intro_scene("intro");
            IntroScene intro_handler(intro_scene, audio_module);

            auto _ireg = intro_handler.registry();
            _ireg->add_system<std::tuple<int>>(std::make_shared<onHoverSystem>(_ireg, _win));
            _ireg->add_system<int>(std::make_shared<BJENGINE::ECS::BUILTINS::SYSTEMS::Input>(_ireg, _win, intro_handler._player_idx));
            _ireg->add_system<bool>(std::make_shared<BJENGINE::Render>(_ireg, _win));
            _ireg->init_systems();

            bool init= true, init_menu = true;
            while (_win.isOpen())
            {
                if (!intro_handler._ended) 
                {
                    intro_handler();
                }
                else if (
                    menu_handler._scene->isActive()) {
                    if (init_menu)
                    {
                        menu_handler._audio->playMusic<std::string>();
                        init_menu = false;
                    }
                    menu_handler();
                }
                else {
                    if (init) {
                    menu_handler._audio->stopMusic<std::string>();
                    audio_module->playMusic<float>(true);
                    _greg->add_system<int>(std::make_shared<BJENGINE::ECS::BUILTINS::SYSTEMS::Input>(_greg, _win, game_handler._player_idx));
                    _greg->add_system<float>(std::make_shared<GameUpdateSystem>(_greg, game_handler, net_module, input->_server_ip, input->_server_port));
                    _greg->add_system<char>(std::make_shared<ParallaxSystem>(_greg));
                    _greg->add_system<bool>(std::make_shared<BJENGINE::Render>(_greg, _win));

                    _greg->init_systems();
                    init = false;
                    }
                    game_handler();
                }
            }
            /*Disconnect*/
            Net::Packet deco;
            deco.id = MsgType::DeconnectionRequest;
            if (!input->_server_ip.empty() && input->_server_port > 0)
                net_module->send(deco, boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(input->_server_ip), input->_server_port));
        }

    private:
        std::unordered_map<std::string, std::function<void()>> _scenes_handler;
        std::vector<BJENGINE::Scene> _scenes;
        sf::RenderWindow _win;
        int _port;
    };

}