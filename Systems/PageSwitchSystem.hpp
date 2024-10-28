#pragma once
#include "../Engine/Builtins/Components/input.hpp"
#include "../Engine/Builtins/Components/rectangularShape.hpp"
#include "../Engine/Builtins/Systems/Render.hpp"
#include "../Components/pressable.hpp"
#include "../Components/net.hpp"
#include "../Components/room.hpp"
#include "../Scenes/Menu.hpp"
#include "../Modules/Data.hpp"
#include "../Client/NetworkModule.hpp"
#include "../Engine/Audio/Audio.hpp"
#include "../constants/constants.hpp"
#include "../Components/lobbyPlayer.hpp"
#include "../Modules/miscellaneous.hpp"

namespace RTYPE
{
    class PageSwitchSystem : public BJENGINE::ASystem
    {
    public:
        PageSwitchSystem(std::shared_ptr<BJENGINE::registry> reg, RTYPE::Menu &menu, std::shared_ptr<NetworkModule> netmodule, std::shared_ptr<AudioManager> audiomodule) : ASystem(reg), _ref_menu(std::make_shared<RTYPE::Menu>(menu)), _net_module(netmodule), _audio_module(audiomodule)
        {
        }

        void start() override
        {
            _registry->register_component<net>();
            _registry->register_component<lobbyPlayer>();
            _registry->attach_component<net>(_ref_menu->_player_idx, net());
            auto navigateToLogin = [this]
            {
                auto entities = _registry->entities();
                for (auto j : entities)
                {
                    if (j != _ref_menu->_player_idx)
                        _registry->deActivateEntity(j);
                }
                _ref_menu->createLoginPage();
            };

            auto createRoom = [this]
            {
                auto &pipeline = (_registry->get_components<net>())[_ref_menu->_player_idx];
                Net::Packet packet;
                packet.id = MsgType::CreateRoom;
                _net_module->send(packet, pipeline->_server_ip, pipeline->_server_port);
            };

            auto joinRoom = [this]
            {
                auto &rooms = _registry->get_components<room>();
                int selected = -1;
                for (auto i : _registry->entities())
                {
                    if (!rooms.contains(i) || !rooms[i].has_value())
                        continue;
                    LOGGER(std::to_string(i));
                    if (!rooms[i].has_value())
                        continue;
                    if (rooms[i]->_selected)
                    {
                        selected = rooms[i]->_id;
                        break;
                    }
                }
                auto &pipeline = (_registry->get_components<net>())[_ref_menu->_player_idx];
                Net::Packet packet;
                packet.id = MsgType::JoinRoom;
                LOGGER(std::to_string(selected));
                packet.body = {static_cast<uint8_t>(selected)};
                _net_module->send(packet, pipeline->_server_ip, pipeline->_server_port);
                while ((_net_module->fetch(true)).id != MsgType::OK);
                auto entities = _registry->entities();
                for (auto j : entities)
                {
                    if (j != _ref_menu->_player_idx)
                        _registry->deActivateEntity(j);
                }
                _ref_menu->createLobbyPage();
                listLobbyPlayers();
            };

            auto leaveRoom = [this]
            {
                auto &pipeline = (_registry->get_components<net>())[_ref_menu->_player_idx];
                Net::Packet packet;
                packet.id = MsgType::LeaveRoom;
                _net_module->send(packet, pipeline->_server_ip, pipeline->_server_port);
                navigateToRooms();
            };

            auto setReady = [*this]
            {
                auto &pipeline = (_registry->get_components<net>())[_ref_menu->_player_idx];
                Net::Packet packet;
                packet.id = MsgType::Ready;
                _net_module->send(packet, pipeline->_server_ip, pipeline->_server_port);                
                while ((_net_module->fetch(true)).id != MsgType::GameStart);
                _ref_menu->_scene->setActive(false);
            };

        auto navigateToHome = [this]
        {

            auto entities = _registry->entities();
            for (auto j : entities)
            {
                if (j != _ref_menu->_player_idx)
                    _registry->deActivateEntity(j);
            }
            _ref_menu->createHomePage();
        };

        auto navigateToOptions = [this]
        {
            auto entities = _registry->entities();
            for (auto j : entities)
            {
                if (j != _ref_menu->_player_idx)
                    _registry->deActivateEntity(j);
            }
            _ref_menu->createOptionPage();
        };


        auto navigateToRoomsWrapper = [this]
        {
            navigateToRooms();
        };

        auto mute = [this]
        {
            _ref_menu->_audio->stopMusic<std::string>();
        };

        auto unmute = [this]
        {
            _ref_menu->_audio->playMusic<std::string>(true);
        };

            _call_backs[RTYPE::Menu::INPUT_TYPE::PLAY] = navigateToLogin;
            _call_backs[RTYPE::Menu::INPUT_TYPE::CREATE] = createRoom;
            _call_backs[RTYPE::Menu::INPUT_TYPE::JOIN] = joinRoom;
            _call_backs[RTYPE::Menu::INPUT_TYPE::LEAVE] = leaveRoom;
            _call_backs[RTYPE::Menu::INPUT_TYPE::READY] = setReady;

            _call_backs[RTYPE::Menu::INPUT_TYPE::BACK_HOME] = navigateToHome;
            _call_backs[RTYPE::Menu::INPUT_TYPE::BACK_LOGIN] = navigateToLogin;
            _call_backs[RTYPE::Menu::INPUT_TYPE::BACK_ROOMS] = navigateToRoomsWrapper;


            _call_backs[RTYPE::Menu::INPUT_TYPE::OPT] = navigateToOptions;

            _call_backs[RTYPE::Menu::INPUT_TYPE::MUTE] = mute;
            _call_backs[RTYPE::Menu::INPUT_TYPE::UNMUTE] = unmute;
        }
        void stop() override
        {
        }

        void navigateToRooms()
        {
            auto entities = _registry->entities();
            for (auto j : entities)
            {
                if (j != _ref_menu->_player_idx)
                    _registry->deActivateEntity(j);
            }
            _ref_menu->createRoomsPage();
            listRooms();
        }

        void listRooms()
        {
            auto &pipeline = (_registry->get_components<net>())[_ref_menu->_player_idx];
            Net::Packet packet;
            packet.id = MsgType::ListRoom;
            _net_module->send(packet, pipeline->_server_ip, pipeline->_server_port);

            auto &rooms = _registry->get_components<room>();

            Net::Packet res;

            if ((res = _net_module->fetch(true)).id != MsgType::RoomList)
                return;
            bool found = false;
            for (auto id : res.body)
            {
                found = false;
                for (auto i : _registry->entities())
                {
                    if (!rooms.contains(i) || !rooms[i].has_value())
                        continue;
                    if (rooms[i]->_id == id)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    _ref_menu->instantiateRoom(id);
                }
            }
        }

        void listLobbyPlayers()
        {
            auto &pipeline = (_registry->get_components<net>())[_ref_menu->_player_idx];
            Net::Packet packet;
            packet.id = MsgType::ListRoomUser;
            _net_module->send(packet, pipeline->_server_ip, pipeline->_server_port);

            auto &players = _registry->get_components<lobbyPlayer>();

            Net::Packet res;

            if ((res = _net_module->fetch(true)).id != MsgType::RoomUserList)
                return;
            bool found = false;
            std::vector<std::string> names = MISCELLANEOUS::split(MISCELLANEOUS::bytesToString(res.body), ' ');
            for (auto name : names)
            {
                found = false;
                for (auto i : _registry->entities())
                {
                    if (!players.contains(i) || !players[i].has_value())
                        continue;
                    if (players[i]->_name == name)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    _ref_menu->instantiateLobbyPlayer(name);
                }
            }
            
        }
        void update() override
        {
            auto &input = *(_registry->get_components<BJENGINE::inputComponent>())[_ref_menu->_player_idx];
            auto &texts = _registry->get_components<textComponent>();

            auto &rectangles = _registry->get_components<rectangularShape>();
            auto &pressables = _registry->get_components<pressable>();
            auto &validationButtons = _registry->get_components<validationButton>();

            auto &pipeline = (_registry->get_components<net>())[_ref_menu->_player_idx];
            Net::Packet packets = _net_module->fetch(false);
            if (packets.id == MsgType::RoomNewMessage) {
                _ref_menu->instantiateMessage(MISCELLANEOUS::bytesToString(packets.body));
            }

            else if (packets.id == MsgType::NewRoomCreated) {
                    _audio_module->playSound<int>();
                listRooms();
            }
            else if (packets.id == MsgType::NewPlayerAddToRoom) {
                    _audio_module->playSound<int>();
                listLobbyPlayers();
            }

            auto &rooms = _registry->get_components<room>();

            if (input._eventType == BJENGINE::INPUT_TYPE::mouseClicked)
            {
                _audio_module->playSound<char>();
                for (auto i : _registry->entities())
                {
                    if (rectangles.contains(i) && pressables.contains(i) && rooms.contains(i) && rectangles[i].has_value() && pressables[i].has_value() && rooms[i].has_value())
                    {
                        if ((*rectangles[i])._rectangle.getGlobalBounds().contains(sf::Vector2f(input._click_coords.x, input._click_coords.y)))
                        {
                            for (auto j : _registry->entities())
                            {
                                if (!rooms.contains(j) || !rooms[j].has_value())
                                    continue;
                                rectangles[j]->_rectangle.setOutlineThickness(1);
                                rooms[j]->_selected = false;
                            }
                            rectangles[i]->_rectangle.setOutlineThickness(6);
                            rooms[i]->_selected = true;
                            input.clear();
                        }
                    }

                    else if (rectangles.contains(i) && pressables.contains(i) && rectangles[i].has_value() && pressables[i].has_value())
                    {
                        if ((*rectangles[i])._rectangle.getGlobalBounds().contains(sf::Vector2f(input._click_coords.x, input._click_coords.y)))
                        {
                            if (_ref_menu->_input_entities.contains(i))
                            {
                                _call_backs[_ref_menu->_input_entities[i]]();
                            }
                            input.clear();
                        }
                    }

                    else if (rectangles.contains(i) && rectangles[i].has_value() && validationButtons.contains(i) && validationButtons[i].has_value())
                    {
                        if ((*rectangles[i])._rectangle.getGlobalBounds().contains(sf::Vector2f(input._click_coords.x, input._click_coords.y)))
                        {
                            if (validationButtons[i]->_name == "login") {
                                auto req = validationButtons[i]->_inputs;
                                pipeline->_server_ip = (std::string)texts[req[1]]->_text.getString();
                                pipeline->_server_port = std::stoi((std::string)texts[req[2]]->_text.getString());
                                LOGGER( pipeline->_server_ip);
                                LOGGER( pipeline->_server_port);
                                Net::Packet packet = MISCELLANEOUS::stringToBytes((std::string)texts[req[0]]->_text.getString());;
                                packet.id = MsgType::ConnectionRequest;
                                
                                _net_module->send(packet, pipeline->_server_ip, pipeline->_server_port);
                                if (_net_module->fetch(true).id == MsgType::ConnecionEstablished)
                                    navigateToRooms();
                            } else if (validationButtons[i]->_name == "chat")
                            {
                                auto req = validationButtons[i]->_inputs;
                                std::string msg = (std::string)texts[req[0]]->_text.getString();
                                Net::Packet packet = MISCELLANEOUS::stringToBytes(msg);
                                packet.id = MsgType::RoomSendMessage;
                                _net_module->send(packet, pipeline->_server_ip, pipeline->_server_port);
                                texts[req[0]]->_text.setString("");
                            }
                            input.clear();

                        }
                    }
                }
            }
        }

    private:
        std::shared_ptr<RTYPE::Menu> _ref_menu;
        std::unordered_map<RTYPE::Menu::INPUT_TYPE, std::function<void()>> _call_backs;
        std::shared_ptr<NetworkModule> _net_module;
        std::shared_ptr<AudioManager> _audio_module;
    };
}
