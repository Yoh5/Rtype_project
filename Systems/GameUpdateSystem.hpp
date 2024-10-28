#pragma once
#include "../Engine/Builtins/Components/input.hpp"
#include "../Engine/Builtins/Components/orientation.hpp"
#include "../Components/networkedObject.hpp"
#include "../Scenes/Game.hpp"
#include "../Client/NetworkModule.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_set>

class GameUpdateSystem : public BJENGINE::ASystem
{
    public:
    
enum entityStatus : uint8_t {
    CREATE,
    UPDATE,
    DEL,
    UNCHANGE,
};

    GameUpdateSystem(std::shared_ptr<BJENGINE::registry> reg, Game &game, std::shared_ptr<NetworkModule> net, std::string ip, int port) :
    ASystem(reg), _game(game), _net(net), _server_endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(ip), port))
    {
        
    }

    void start() override
    {
        _registry->register_component<networkedObject>();
        _registry->register_component<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>();
        _registry->register_component<transform>();
        _inputs_packets = 
        {
            {122, MsgType::MoveUp},
            {113, MsgType::MoveLeft},
            {115, MsgType::MoveDown},
            {100, MsgType::MoveRight},
            {32, MsgType::Fire},
        };
        _dead = false;
    }

    void update() override
    {
        auto &input = _registry->get_components<BJENGINE::inputComponent>()[_game._player_idx];

        if (input->_eventType == BJENGINE::INPUT_TYPE::keyPressed && _inputs_packets.contains(input->_key_code) && !_dead)
        {
            Net::Packet packet;

            packet.id = _inputs_packets[input->_key_code];

            _net->send(packet, _server_endpoint);
            input->clear();
        }
        handleNotif(_net->fetch(false));
    }

    void handleNotif(Net::Packet data)
    {
    if (data.id == MsgType::Stats)
    {
        int _health_ = data.body[0];
        int _score_ = (static_cast<uint16_t> (data.body[2]) <<  8 ) | data.body[1];
        int _level_ = data.body[3];
        if (_health_ <= 0)
            _dead = true;
        auto &hp = _registry->get_components<textComponent>()[_game._hp];

        auto &score = _registry->get_components<textComponent>()[_game._score];

        auto &level = _registry->get_components<textComponent>()[_game._level];
        hp->_text.setString(std::to_string(_health_));
        score->_text.setString(std::to_string(_score_));
        level->_text.setString(std::to_string(_level_));
    }
    else if (data.id  == MsgType::Win)
    {
        _game.createWinPage();
        _game._audio->playSound<float>();
    }
    else if (data.id  == MsgType::Loose)
    {
        _game.createLosePage();
        _game._audio->playSound<double>();
        
    }
    else if (data.id == MsgType::Info) {
        
        int id = (static_cast<uint16_t> (data.body[1]) <<  8 ) | data.body[0];
        int state = data.body[2];
        int asset = data.body[3];
        int orientation = data.body[4];
        int x = (static_cast<uint16_t> (data.body[6]) <<  8 ) | data.body[5];
        int y = (static_cast<uint16_t> (data.body[8]) <<  8 ) | data.body[7];

        if (
            (state == entityStatus::CREATE) && (
            asset == Game::drawable::FIRE1 ||
            asset == Game::drawable::FIRE2 ||
            asset == Game::drawable::FIRE3 ||
            asset == Game::drawable::FIRE4
        ))
            _game._audio->playSound<bool>();
        auto &networked = _registry->get_components<networkedObject>();

        switch (state)
        {
            case entityStatus::CREATE:
            createNetworkEntity(id, (Game::drawable)asset, x, y);
            break;
            case entityStatus::UPDATE:
            updateNetworkEntity(id, x, y);
            break;
            case entityStatus::DEL:
            deleteNetworkEntity(id, x, y);
            break;
        }
    }
    }

    void createNetworkEntity(int net_id, Game::drawable type, int x, int y)
    {
        int idx = _registry->spawn_entity();

        int local_id = _registry->spawn_entity();

        std::unordered_map<std::type_index, std::vector<sf::Sprite>> animation;
        animation[typeid(int)] = _game._sprites[type];
        _registry->attach_component<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>(local_id, BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer(animation, 0.6, typeid(int)));
        _registry->attach_component<transform>(local_id, transform(glm::vec2(x,y)));

        _registry->attach_component<networkedObject>(idx, networkedObject(net_id, local_id));
    }

    void updateNetworkEntity(int net_id, int x, int y)
    {
        auto &networked = _registry->get_components<networkedObject>();
        auto &transforms = _registry->get_components<transform>();

        for (auto i : _registry->entities())
        {
            if (networked.contains(i) && networked[i].has_value())
            {
                if (net_id == networked[i]->_net_id)
                {
                    int to_update = networked[i]->_local_id;
                    if (transforms.contains(to_update))
                        transforms[to_update]->_position = glm::vec2(x, y);
                }
            }
        }
    }

    void deleteNetworkEntity(int net_id, int x, int y)
    {
        LOGGER(x)
        LOGGER(y)
        auto &networked = _registry->get_components<networkedObject>();

        for (auto i : _registry->entities())
        {
            if (networked.contains(i) && networked[i].has_value() && networked[i]->_net_id == net_id)  {    
                _game._audio->playSound<std::string>();
                std::unordered_map<std::type_index, std::vector<sf::Sprite>> animation;
                animation[typeid(int)] = _game._sprites[Game::drawable::EXPLOSION];
                for (auto &sp : animation[typeid(int)])
                    sp.setPosition(sf::Vector2f(x, y));
                int explo = _registry->spawn_entity();
               _registry->attach_component<BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer>(explo, BJENGINE::BUILTINS::COMPONENTS::animatedSpriteRenderer(animation, 0.6, typeid(int), false));
                _registry->deActivateEntity(networked[i]->_local_id);
                _registry->deActivateEntity(i);
            }
        }  
    }

    void stop() override
    {

    }

    private:

    std::shared_ptr<BJENGINE::registry> _reg;
    Game _game;
    std::shared_ptr<NetworkModule> _net;
    boost::asio::ip::udp::endpoint _server_endpoint;
    std::unordered_map<sf::Uint32, MsgType> _inputs_packets;
 
    std::vector<uint16_t> _net_ids; 
    bool _dead;
};