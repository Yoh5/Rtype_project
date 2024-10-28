/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** RtypeServer
*/

#ifndef RTYPESERVER_HPP_
    #define RTYPESERVER_HPP_
    #include "Server.hpp"
    #include "EnemyManager.hpp"
    #include "drawableType.hpp"
    #include "ManageInfo.hpp"
    #include "Destructor.hpp"
    #include "infoPacket.hpp"
    #include "shooterManager.hpp"
    #include "ControlableManager.hpp"
    #include "CollisionEventManager.hpp"
    #include "../../Modules/miscellaneous.hpp"
    #include "../../Engine/scene/scene.hpp"
    #include "../../Engine/Builtins/Systems/Movement.hpp"
    #include "../../Engine/Builtins/Systems/Collision.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/Builtins/Components/transform.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/Builtins/Components/gravity.hpp"
    #include "../../Engine/Builtins/Components/velocity.hpp"
    #include "../../Engine/Builtins/Components/acceleration.hpp"
    #include "../../Engine/Builtins/Components/boxCollider.hpp"
    #define MAX_NUMBER_ROOM 7
    #define CYCLE_SIZE 30

using namespace MISCELLANEOUS;

namespace RtypeNet {

    enum playerState {
        READY,
        INROOM,
        INGAMING,
        CONNECTED,
    };

    enum roomState {
        RECEPTION,
        GAMERUNNING,
    };

    struct playerInfo {
        playerInfo(int i, std::string n) : id(i), name(n) {}
        int id;
        int x = 2;
        int y = 1;
        const int vx = 12;
        const int vy = 12;
        const int health = 100;
        const int damage = 20;
        int roomId = 0;
        int roomPos = -1;
        std::string name;
        playerState state = CONNECTED;
    };

    struct gameRoom {
      //  RtypeNet::gameRoom::gameRoom(RtypeNet::gameRoom&&) = delete;
        gameRoom(size_t m, boost::asio::ip::udp::endpoint e, int c_end) : max(m), playerCreator(e), nbPAlive(m), cycle_end(c_end) {}
        int cycle = 1;
        int cycle_end;
        int max;
        int nbPAlive;
        BJENGINE::Scene scene;
        size_t nbPlayerReady = 0;
        roomState state = roomState::RECEPTION;
        std::unordered_map<int, std::shared_ptr<infoPacket>> infos;
        std::unordered_map<int, std::shared_ptr<infoStats>> stats;
        boost::asio::ip::udp::endpoint playerCreator;
        std::shared_ptr<Thread::ThreadSafeQueue<Net::ownedPacket>> mesgs;
        std::map<boost::asio::ip::udp::endpoint, std::shared_ptr<playerInfo>> players;
    };

    struct MANAGER_TAG
    {
    };

    struct ENEMY_TAG
    {
    };

    struct INFO_TAG
    {
    };

    struct MOVEMENT_CONTROLLER
    {
    };

    struct DESTRUCTION_TAG
    {
    };

    struct SHOOTER_TAG
    {
    };

    struct COLISION_TAG
    {
    };

    struct COLISION_EVENT_TAG
    {
    };

    class RtypeServer : public  Net::ServerUDP {
        public:
            RtypeServer(size_t port, std::string m = "./Server/enemies") : Net::ServerUDP(port), mapDir(m) {
                bestScore = 0;
            }

        protected:
            virtual bool onClientConnect(boost::asio::ip::udp::endpoint client, Net::Packet msg)
	        {
	        	Net::Packet resp;
                if (!MISCELLANEOUS::vector_contains(client.address().to_string(), bannedIp)) {
	        	    resp.id = MsgType::ConnecionEstablished;
	        	    sendMessageToClient(client, resp);
	        	    return true;
                }
                resp.id = MsgType::ConnectionRefused;
                sendMessageToClient(client, resp);
                return false;
	        }

            virtual void onClientDisconnect(boost::asio::ip::udp::endpoint client)
	        {
                if (rooms[players[client]->roomId])
                    rooms[players[client]->roomId]->players.erase(client);
                players.erase(client);
		        std::cout << "[SERVER] Removing client [" << client.address().to_string() << "]" << std::endl;
	        }

            virtual void sendMessageToAllClient(const Net::Packet &msg)
            {
                for (auto [key, value] : players) 
                   sendMessageToClient(key, msg); 
            }

            void sendMessageToAdmin(const Net::Packet &msg)
            {
                communication.send(msg, admin);
            }

            bool onAdminConnect(boost::asio::ip::udp::endpoint admin, Net::Packet msg)
            {
                Net::Packet resp;
                std::vector<std::string> creds = split(bytesToString(msg.body), ' ');
                //std::cout << "[SERVER]: username-> " << creds[0] << " password -> " << creds[1] << std::endl;
                if (creds[0] == "admin" && creds[1] == "password" && !adminConnected) {
                    resp.id = MsgType::AdminConnectionEstablished;
                    this->admin = admin;
                    adminConnected = true;
                    sendMessageToAdmin(resp);
                    return true;
                }
                resp.id = MsgType::AdminConnectionRefused;
                sendMessageToClient(admin, resp);
                return false;
            }

            void onAdminDisconnect()
            {
                std::cout << "[SERVER] admin deconnection." << std::endl;
                adminConnected = false;
                sendMessageToAdmin(MsgType::OK);
            }

            bool isAdmin(boost::asio::ip::udp::endpoint admin)
            {
                if (adminConnected)
                    return (this->admin == admin);
                else
                    return false;
            }

            virtual bool isAClient(boost::asio::ip::udp::endpoint cli_end)
            {
                return players.count(cli_end);
            }

            bool isRoomCommand(MsgType id)
            {
                if (id >= MsgType::CreateRoom && id <= MsgType::Ready)
                    return true;
                return false;
            }

            bool isGameRoomCommand(MsgType id)
            {
                if (id >= MsgType::Info && id <= MsgType::MoveDown)
                    return true;
                return false;
            }

            virtual void onMessage(boost::asio::ip::udp::endpoint client, Net::Packet msg)
	        {
                //std::cout << "New Message Received" << std::endl;
                if (isAClient(client)) {
                    // Process Client Data
                   // std::cout << "[SERVER] message id " << (int)msg.id << std::endl;
                    if (isRoomCommand(msg.id)) roomCommandManager(client, msg);
                    else if (isGameRoomCommand(msg.id)) gameRoomManager(client, msg);
                    else if (msg.id == MsgType::DeconnectionRequest) onClientDisconnect(client);
                    else {
                        std::cout << "[SERVER] Invalid Command" << std::endl;
                        Net::Packet msg;
                        msg.id = MsgType::UnvalidMessage;
                        sendMessageToClient(client, msg);
                    }
                } else if (isAdmin(client)) {
                    manageAdminCommand(msg);
                } else {
                    if (msg.id == MsgType::ConnectionRequest) {
                        if (onClientConnect(client, msg)) {
                            std::string n = bytesToString(msg.body);
                            std::cout << "[SERVER]: New client registered : " << client.address().to_string() << "name :" << n << std::endl;
                            players[client] = std::make_shared<playerInfo>(playerInfo(idCounter++, n));
                        }
                    } else if (msg.id == MsgType::AdminConnectionRequest) {
                        if (onAdminConnect(client, msg)) {
                            std::cout << "[SERVER]: Admin connected : " << client.address().to_string() << std::endl;
                        }
                    }  else {
                        std::cout << "[SERVER] Bad request" << std::endl;
                        Net::Packet msg;
                        msg.id = MsgType::ConnectionRequire;
                        sendMessageToClient(client, msg);
                    }
                }
    	    }

            void sendToAllUsersInRoom(size_t roomId, Net::Packet msg)
            {
                for (auto [k, info] : rooms[roomId]->players) {
                    sendMessageToClient(k, msg);
                }
            }

            void sendToAllUsersInState(playerState state, Net::Packet msg)
            {
                for (auto player : players) {
                    if (player.second->state == state)
                        sendMessageToClient(player.first, msg);
                }
            }

//////////////////////////////////////// Room Commands //////////////////////////////////////////////////////////////////////////

            void createRoom(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                if (rooms.size() == MAX_NUMBER_ROOM) return;
                std::cout << "[SERVER]: room " << roomCounter << " created" << std::endl;
                rooms[roomCounter++] = std::make_shared<gameRoom>(gameRoom(this->maxRoom, client, this->cycle));
                sendToAllUsersInState(playerState::CONNECTED, MsgType::NewRoomCreated);
            }

            void addPlayerToRoom(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                // msg : |msg id|room id|
                size_t roomId = msg.body[0];
                std::cout << "[SERVER] adding player " << players[client]->id << " to room " << roomId << std::endl;
                if (!rooms.count(roomId)) {
                    sendMessageToClient(client, Net::Packet(MsgType::RoomFull));
                    std::cerr << "[SERVER] Bad room Id" << std::endl;
                    return;
                }
                if (players[client]->state == playerState::CONNECTED && rooms[roomId]->players.size() < rooms[roomId]->max && rooms[roomId]->state != roomState::GAMERUNNING) {
                    players[client]->roomId = roomId;
                    players[client]->state = playerState::INROOM;
                    rooms[roomId]->players[client] = players[client];
                    players[client]->roomPos = rooms[roomId]->players.size();
                    players[client]->y += (rooms[roomId]->players.size()) * 100;
                    Net::Packet resp;
                    Net::Packet notif;
                    notif.id = MsgType::NewPlayerAddToRoom;
                    resp.id = MsgType::OK;
                    sendMessageToClient(client, resp);
                    sendToAllUsersInRoom(roomId, notif);
                }
            }

            void deletePlayerFromRoom(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                rooms[players[client]->roomId]->players.erase(client);
                players[client]->roomId = -1;
            }

            void managePlayerReady(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                std::cout << "[SERVER]: Player " << players[client]->id << " ready" << std::endl;
                if (players[client]->state == playerState::INROOM) {
                    players[client]->state = playerState::READY;
                    rooms[players[client]->roomId]->nbPlayerReady++;
                    std::cout << "[SERVER]: Now " << rooms[players[client]->roomId]->nbPlayerReady << " Player ready" << std::endl;
                    std::cout << "[SERVER]: Remain Player ready " << rooms[players[client]->roomId]->max - rooms[players[client]->roomId]->nbPlayerReady << std::endl;
                    if (rooms[players[client]->roomId]->max == rooms[players[client]->roomId]->nbPlayerReady) {
                        Net::Packet resp;
                        resp.id = MsgType::GameStart;
                        for (auto [k, pinfo] : rooms[players[client]->roomId]->players) {
                            pinfo->state = playerState::INGAMING;
                            sendMessageToClient(k, resp);
                        }
                        rooms[players[client]->roomId]->state = roomState::GAMERUNNING;
                        rooms[players[client]->roomId]->mesgs = std::make_shared<Thread::ThreadSafeQueue<Net::ownedPacket>>();
                        std::cout << "[SERVER] room initialisation" << std::endl;
                        initRoom = [this, client] () {
                            std::cout << "[SERVER]: HIHO DELA PUTA" << std::endl;
                            runRoom(players[client]->roomId);
                       };
                       roomThreads.enqueue(initRoom);
                    }
                }
            }

            void deleteRoom(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                // msg : |msg id|room id|
                size_t roomId = msg.body[0];
                Net::Packet response;
                response.id = MsgType::OK;
                if (rooms.count(roomId) || client == rooms[roomId]->playerCreator) {
                    rooms.erase(roomId);
                    sendMessageToClient(client, response);
                }
            }

            void listRoom(boost::asio::ip::udp::endpoint client)
            {
                Net::Packet m;
                m.id = MsgType::RoomList;
                std::cout << "Room list" << std::endl;
                for (auto [k, value] : rooms) {
                    m << (uint8_t) k;
                }
                sendMessageToClient(client, m);
            }

            void listPlayerReady(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                // msg : |msg id|room id|
                size_t roomId = msg.body[0];
                // parcour list of player of rooms[roomid] and get identifier of all them that have ready state
            }

            void sendMessageInRoom(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                if (players[client]->state == playerState::INROOM) {
                    msg.id = MsgType::RoomNewMessage;
                    msg << (uint8_t)players[client]->roomPos;
                    for (auto [r_client, k] : rooms[players[client]->roomId]->players)
                        sendMessageToClient(r_client, msg);
                }
            }

            void listRoomUser(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                Net::Packet m;
                std::string names;
                for (auto [id, info] : rooms[players[client]->roomId]->players) {
                    names += info->name;
                    names += " ";
                }
                names.pop_back();
                m = stringToBytes(names);
                m.id = MsgType::RoomUserList;
                sendMessageToClient(client, m);
            }

            void roomCommandManager(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                if (msg.id == MsgType::CreateRoom) createRoom(client, msg);
                else if (msg.id == MsgType::JoinRoom) addPlayerToRoom(client, msg); // Function to add a new player to room he select
                else if (msg.id == MsgType::LeaveRoom) deletePlayerFromRoom(client, msg);
                else if (msg.id == MsgType::DeleteRoom) deleteRoom(client, msg);
                else if (msg.id == MsgType::Ready) managePlayerReady(client, msg);
                else if (msg.id == MsgType::ListRoom) listRoom(client);
                else if (msg.id == MsgType::ListPlayerReady) listPlayerReady(client, msg);
                else if (msg.id == MsgType::ListRoomUser) listRoomUser(client, msg); 
                else if (msg.id == MsgType::RoomSendMessage) sendMessageInRoom(client, msg);
            }

///////////////////////////////////////////////////// Room Commands //////////////////////////////////////////////////////////////////////////


        protected:
    
            unsigned long long getTotalPackets() const {
                return totalPacketsSent;
            }

            void getAllTotalPackets(void)
            {
                unsigned long long totalPackets = getTotalPackets();
                Net::Packet response;
                response.id = MsgType::TotalPackets;
                response << totalPackets;
                sendMessageToAdmin(response);
            }

            void listConnectedUser(void)
            {
                Net::Packet msg;
                msg.id = MsgType::ConnectedUsers;
                for (auto [user, info] : players)
                    msg << info->id;
                sendMessageToAdmin(msg);
            }
            
            void getUserInfo(Net::Packet msg)
            {
                Net::Packet resp;
                size_t id = msg.body[0];
                resp.id = MsgType::UserInfo;
                for (auto [k, info] : players)
                    if (info->id == id) { 
                        resp.body = stringToBytes(k.address().to_string() + " "  + info->name).body;
                        break;
                    }
                sendMessageToAdmin(resp);
            }

            void adminListRoom(void)
            {
                listRoom(admin);
            }

            void getInfoRoom(Net::Packet msg)
            {
                size_t roomId = msg.body[0];
                Net::Packet resp;
                resp.id = MsgType::RoomInfo;
                resp << rooms[roomId]->max << rooms[roomId]->nbPlayerReady << rooms[roomId]->state << rooms[roomId]->playerCreator;
                sendMessageToAdmin(resp);
            }

            void banUser(Net::Packet msg)
            {
                size_t userId = msg.body[0];
                for (auto [key, info] : players)
                    if (info->id == userId) {
                        bannedIp.push_back(key.address().to_string());
                        Net::Packet resp;
                        resp.id = MsgType::UserBanned;
                        sendMessageToClient(key, resp);
                        onClientDisconnect(key);
                        break;
                    }
            }

            void debanUser(Net::Packet msg)
            {
                std::string b_ip = bytesToString(msg.body);
                for (int i = 0, size = bannedIp.size(); i < size; i++) {
                    if (bannedIp[i] == b_ip)
                        bannedIp.erase(bannedIp.begin() + i);
                }
            }

            void listUserBanned(void)
            {
                Net::Packet resp;
                resp.id = MsgType::UserBannedList;
                for (auto ip : bannedIp)
                    resp << ip.c_str();
                sendMessageToAdmin(resp);
            }

            void setMaxRoom(Net::Packet msg)
            {
                size_t m = msg.body[0];
                if (m > 4)
                    m = 4;
                this->maxRoom = m;
            }

            void setCycle(Net::Packet msg)
            {
                size_t m = msg.body[0];
                this->cycle = m;
            }

            void manageAdminCommand(Net::Packet msg)
            {
                if (msg.id == MsgType::ListConnectedUser) listConnectedUser();
                if (msg.id == MsgType::InfoUser) getUserInfo(msg);
                if (msg.id == MsgType::AdminListRoom) adminListRoom();
                if (msg.id == MsgType::InfoRoom) getInfoRoom(msg);
                if (msg.id == MsgType::BanUser) banUser(msg);
                if (msg.id == MsgType::DeBan) debanUser(msg);
                if (msg.id == MsgType::ListUserBanned) listUserBanned();
                if (msg.id == MsgType::SETMAXROOM) setMaxRoom(msg);
                if (msg.id == MsgType::SETCYCLE) setCycle(msg);
                if (msg.id == MsgType::AdminDeconnectionRequest) onAdminDisconnect();
            }

        protected:

            void gameRoomManager(boost::asio::ip::udp::endpoint client, Net::Packet msg)
            {
                //std::cout << "[SERVER]: Game message processing" << std::endl;
                if (rooms[players[client]->roomId]->state != roomState::RECEPTION) 
                    rooms[players[client]->roomId]->mesgs->push_back({client, msg});
            }

            void updateInfo(size_t id)
            {
                auto _registry = rooms[id]->scene.get_registry();
                auto &drawables = _registry->get_components<drawable>();
                auto &transforms = _registry->get_components<transform>();
                auto &orientations = _registry->get_components<orientation>();
                auto &stats = _registry->get_components<property>();
                auto newlist = _registry->entities();
                for (auto i : newlist) {
                    if (drawables.contains(i) && transforms.contains(i) && orientations.contains(i)
                        && drawables[i].has_value() && transforms[i].has_value() && orientations[i].has_value()) {
                        if (!rooms[id]->infos.count(i)) {
                            
                            rooms[id]->infos[i] = std::make_shared<infoPacket>();
                            rooms[id]->infos[i]->ort =  orientations[i]->_orientation;
                            rooms[id]->infos[i]->pos = transforms[i]->_position;
                            rooms[id]->infos[i]->type = drawables[i]->_type;
                            rooms[id]->infos[i]->status = entityStatus::CREATE;
                           // std::cout << "[SERVER] id entity:" << i << "entity size " <<rooms[id]->infos->size() << std::endl;
                        } else if (rooms[id]->infos.count(i) && (rooms[id]->infos[i]->pos != transforms[i]->_position || rooms[id]->infos[i]->ort != orientations[i]->_orientation)) {
                            rooms[id]->infos[i]->ort =  orientations[i]->_orientation;
                            rooms[id]->infos[i]->pos = transforms[i]->_position;
                            rooms[id]->infos[i]->type = drawables[i]->_type;
                            rooms[id]->infos[i]->status = entityStatus::UPDATE;
                        } else if (rooms[id]->infos.count(i) && (rooms[id]->infos[i]->pos == transforms[i]->_position && rooms[id]->infos[i]->ort == orientations[i]->_orientation))
                            rooms[id]->infos[i]->status = entityStatus::UNCHANGE;
                    }
                    if (stats.contains(i) &&  drawables.contains(i) && stats[i].has_value() && drawables[i].has_value()) {
                        if (drawables[i]->_type < 5) {
                            if (!rooms[id]->stats.count(i)) {
                                rooms[id]->stats[i] = std::make_shared<infoStats>();
                                rooms[id]->stats[i]->health = stats[i]->health;
                                rooms[id]->stats[i]->score = stats[i]->score;
                                rooms[id]->stats[i]->status = entityStatus::CREATE;
                                rooms[id]->stats[i]->player = (int)drawables[i]->_type;
                            } else if (rooms[id]->stats.count(i) && (rooms[id]->stats[i]->health != stats[i]->health || rooms[id]->stats[i]->score != stats[i]->score)) {
                                rooms[id]->stats[i]->health = stats[i]->health;
                                rooms[id]->stats[i]->score = stats[i]->score;
                                rooms[id]->stats[i]->status = entityStatus::CREATE;
                                rooms[id]->stats[i]->player = (int)drawables[i]->_type;
                            } else if (rooms[id]->stats.count(i) && (rooms[id]->stats[i]->health == stats[i]->health && rooms[id]->stats[i]->score == stats[i]->score))
                                rooms[id]->stats[i]->status = entityStatus::UNCHANGE;
                        }
                    }
                }
                std::vector<int> to_del;
                for (auto info : rooms[id]->infos) {
                    if (info.second->status == entityStatus::DEL)
                        to_del.push_back(info.first);
                    else if (!MISCELLANEOUS::vector_contains(info.first, newlist)) {
                        rooms[id]->infos[info.first]->status = entityStatus::DEL;
                    }
                }
                for (int i : to_del)
                    rooms[id]->infos    .erase(i);
                to_del.clear();

                for (auto st : rooms[id]->stats) {
                    if (st.second->status == entityStatus::DEL)
                        to_del.push_back(st.first);
                    else if (!MISCELLANEOUS::vector_contains(st.first, newlist)) {
                        rooms[id]->stats[st.first]->status = entityStatus::DEL;
                        rooms[id]->stats[st.first]->health = 0;
                        rooms[id]->nbPAlive--;
                    }
                }
                
                for (int i : to_del)
                    rooms[id]->stats.erase(i);
            }

            void sendUpdateInfo(size_t id)
            {
                //std::cout << "[SERVER] Info sending" << std::endl;
                for (auto [k, st] : rooms[id]->stats) {
                    if (st->status != entityStatus::UNCHANGE) {
                        for (auto [client, cli_inf] : rooms[id]->players) {
                            if (cli_inf->roomPos == st->player) {
                                Net::Packet msg;
                                msg.id = MsgType::Stats;
                                msg << (uint8_t) st->health;
                                msg << (uint16_t) st->score;
                                msg << rooms[id]->cycle;
                                std::cout << "health = " << st->health << " score = " << st->score << " cycle = " << rooms[id]->cycle << std::endl;
                                sendMessageToClient(client, msg);
                            }
                        }
                    }
                }

                for (auto [k, info] : rooms[id]->infos) {
                    if (info->status != entityStatus::UNCHANGE)
                        for (auto [client, i] : rooms[id]->players) {
                            Net::Packet msg;
                            msg.id = MsgType::Info;
                            msg << (uint16_t)k;
                            msg << (uint8_t)info->status;
                            msg << (uint8_t)info->type;
                            msg << (uint8_t)info->ort;
                            msg << (uint16_t)info->pos.x;
                            msg << (uint16_t)info->pos.y;
                          //  std::cout << "x = " << info->pos.y << "y = " << info->pos.y << std::endl;
                            sendMessageToClient(client, msg);
                        }
                }
            }

            void manageRoomPlayerRequest(size_t id)
            {
                auto reg = rooms[id]->scene.get_registry();
                auto &drawables = reg->get_components<drawable>();
                auto &controlables = reg->get_components<controlable>();
                while (!rooms[id]->mesgs->empty()) {
                    auto req = rooms[id]->mesgs->pop_front();
                    if (req.msg.id == MsgType::MoveUp){
                        for (auto i : reg->entities())
                            if (drawables.contains(i) && drawables[i].has_value() && drawables[i]->_type == rooms[id]->players[req.endpoint]->roomPos) {
                                std::cout << "[SERVER]: Move Up received" << std::endl;
                                controlables[i]->lastKeyType = key::UP;
                            }
                    } if (req.msg.id == MsgType::MoveDown) {
                        for (auto i : reg->entities())
                            if (drawables.contains(i) && drawables[i].has_value() && drawables[i]->_type == rooms[id]->players[req.endpoint]->roomPos) {
                                std::cout << "[SERVER]: Move Down received" << std::endl;
                                controlables[i]->lastKeyType = key::DOWN;
                            }
                    } if (req.msg.id == MsgType::MoveLeft) {
                        for (auto i : reg->entities())
                            if (drawables.contains(i) && drawables[i].has_value() && drawables[i]->_type == rooms[id]->players[req.endpoint]->roomPos) {
                                std::cout << "[SERVER]: Move left received" << std::endl;
                                controlables[i]->lastKeyType = key::LEFT;
                            }
                    } if (req.msg.id == MsgType::MoveRight) {
                        for (auto i : reg->entities())
                            if (drawables.contains(i) && drawables[i].has_value() && drawables[i]->_type == rooms[id]->players[req.endpoint]->roomPos) {
                                std::cout << "[SERVER]: Move right received" << std::endl;
                                controlables[i]->lastKeyType = key::RIGHT;
                            }
                    } if (req.msg.id == MsgType::Fire) {
                        for (auto i : reg->entities())
                            if (drawables.contains(i) && drawables[i].has_value() && drawables[i]->_type == rooms[id]->players[req.endpoint]->roomPos) {
                                std::cout << "[SERVER]: Fire received" << std::endl;
                                controlables[i]->lastKeyType = key::FIRE;
                            }
                    }
                }
            }

            void initRoomPlayer(size_t rid, boost::asio::ip::udp::endpoint pid)
            {
                auto _reg = rooms[rid]->scene.get_registry();
                auto player = _reg->spawn_entity();
                glm::vec2 pos(rooms[rid]->players[pid]->x, rooms[rid]->players[pid]->y);
                glm::vec2 vc(rooms[rid]->players[pid]->vx, rooms[rid]->players[pid]->vy);
                _reg->register_component<transform>();
                _reg->register_component<velocity>();
                _reg->register_component<acceleration>();
                _reg->register_component<orientation>();
                _reg->register_component<gravity>();
                _reg->register_component<controlable>();
                _reg->register_component<boxCollider>();
                _reg->register_component<property>();
                auto c = boxCollider(0, 0, P_HEIGHT, P_WIDTH);
                c.tag = "player";
                property p(rooms[rid]->players[pid]->health, rooms[rid]->players[pid]->damage);
                _reg->attach_components(player, transform(pos), velocity(vc), orientation(ORIENTATION::Immobile), acceleration(), gravity(), drawable((drawableType)rooms[rid]->players[pid]->roomPos), controlable(), c, p); 
            }

            void gameRoomInitialisation(size_t id)
            {
                auto _reg = rooms[id]->scene.get_registry();
                _reg->register_component<drawable>();
                _reg->add_system<MOVEMENT_CONTROLLER>(std::make_shared<ControlableManager>(_reg));
                _reg->add_system<SHOOTER_TAG>(std::make_shared<ShooterManager>(_reg));
                _reg->add_system<MANAGER_TAG>(std::make_shared<Movement>(_reg));
                _reg->add_system<DESTRUCTION_TAG>(std::make_shared<Destructor>(_reg));
                _reg->add_system<ENEMY_TAG>(std::make_shared<EnemyManager>(_reg, rooms[id]->cycle, mapDir));
                _reg->add_system<COLISION_TAG>(std::make_shared<CollisionSystem>(_reg));
                _reg->add_system<COLISION_EVENT_TAG>(std::make_shared<CollisionEventManager>(_reg));
                _reg->init_systems();
            }
            
            void runRoom(size_t id)
            {
                std::cout << "[SERVER] room " << id << " started" << std::endl;
                auto cycleStart = std::chrono::high_resolution_clock::now();                
                gameRoomInitialisation(id);
                for (auto [pid, value] : rooms[id]->players) initRoomPlayer(id, pid);
                sendUpdateInfo(id);
                auto sendTime = std::chrono::high_resolution_clock::now();
                while (rooms[id]->state == roomState::GAMERUNNING) {
                    auto now = std::chrono::high_resolution_clock::now();
                    if (!rooms[id]->mesgs->empty()) manageRoomPlayerRequest(id);
                    rooms[id]->scene.run();
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - sendTime).count() >= 80) {
                        updateInfo(id);
                        sendUpdateInfo(id);
                        /*if (rooms[id]->nbPAlive <= 0) {
                            sendToAllUsersInRoom(id, Net::Packet(MsgType::Loose));
                            //std::cout << "[SERVER]: Game Loose" << std::endl;
                        }*/
                        sendTime = std::chrono::high_resolution_clock::now();
                    }
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - cycleStart).count() >= 30000) {
                        rooms[id]->cycle += 1;
                        cycleStart = std::chrono::high_resolution_clock::now();
                        if (rooms[id]->cycle >= rooms[id]->cycle_end)
                            break;
                    }
                }
                if (rooms[id]->nbPAlive > 0) {
                    sendToAllUsersInRoom(id, Net::Packet(MsgType::Win));
                    std::cout << "[SERVER]: Game Win" << std::endl;
                } else {
                    sendToAllUsersInRoom(id, Net::Packet(MsgType::Loose));
                    std::cout << "[SERVER]: Game Loose" << std::endl;
                }
                std::cout << "[SERVER]: Room " << id << " closed." << std::endl;
                rooms[id]->state = roomState::RECEPTION;
                //rooms[id]->cycle = 1;
                //auto e = rooms[id]->playerCreator;
               // rooms[id] = std::make_shared<gameRoom>(this->maxRoom, e, this->cycle);

                //rooms[id] = std::make_shared();
            }

        private:
            size_t idCounter = 1;
            size_t roomCounter = 1;
            size_t maxRoom = 2;
            size_t bestScore;
            std::string mapDir;
            int cycle = 2;
            unsigned long long totalPacketsSent = 0;
            unsigned long long totalPacketsReceived = 0;
            std::map<size_t, std::shared_ptr<gameRoom>> rooms;
            Thread::ThreadPool roomThreads;
            //Thread::ThreadSafeQueue<players>
            std::function<void()> initRoom;
            std::map<boost::asio::ip::udp::endpoint, std::shared_ptr<playerInfo>> players;
            std::vector<std::string> bannedIp;
            boost::asio::ip::udp::endpoint admin;
            bool adminConnected = false;
    };
}

#endif /* !RTYPESERVER_HPP_ */
