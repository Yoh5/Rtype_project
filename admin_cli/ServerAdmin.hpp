#ifndef SERVER_HPP_
    #define SERVER_HPP_
    #include "../Server/include/dependencies.hpp"
    #include "../Modules/Data.hpp"
    #include "../Modules/ThreadPool.hpp"
    #include "../Modules/Network.hpp"
    #include "../Modules/miscellaneous.hpp"

    namespace std {
        template <>
        struct hash<boost::asio::ip::udp::endpoint> {
            size_t operator()(const boost::asio::ip::udp::endpoint& endpoint) const {
                return hash<string>()(endpoint.address().to_string()) ^ (hash<unsigned short>()(endpoint.port()) << 1);
            }
        };
    }
    #include <unordered_set>

namespace Net {

    class AdminServer {
    public:
        AdminServer(size_t port)
            : admin_port(port), communication(asioContext, port) {
            messagesRecv = std::make_shared<Thread::ThreadSafeQueue<ownedPacket>>();
            communication.setMessagesReceive(messagesRecv);
        }

        ~AdminServer() {
            stop();
        }

        bool start() {
            try {
                communication.start();
                threadContext = std::thread([this]() { asioContext.run(); });
            } catch (std::exception &e) {
                std::cerr << "[ADMIN CLI ERROR]: " << e.what() << std::endl;
                return false;
            }
            std::cout << "[ADMIN CLI] Started on port " << admin_port << "." << std::endl;
            return true;
        }

        void stop() {
            asioContext.stop();
            if (threadContext.joinable()) threadContext.join();
            std::cout << "[ADMIN CLI] Stopped." << std::endl;
        }

        void sendMessageToServer(Net::Packet msg)
        {
            communication.send(msg, server);
        }

        void connectToServer(std::string ip, std::string port)
        {
            std::cout << ip << std::endl;
            std::cout << port << std::endl;
            boost::asio::ip::udp::resolver resolver{ asioContext };
            server = *resolver.resolve(boost::asio::ip::udp::v4(), ip, port).begin();
            server_port = std::stoi(port);
        }

        void loginToServer(std::vector<std::string> command)
        {
            if (command.size() != 3) {
                std::cout << "Wrong number of Argument" << std::endl;
                return;
            }
            Net::Packet msg;
            std::string username = command[1];
            std::string password = command[2];
            msg.id = MsgType::AdminConnectionRequest;
            for (auto c : username) msg << c;
            msg << ' ';
            for (auto c : password) msg << c;
            sendMessageToServer(msg);
            if (messagesRecv->empty()) messagesRecv->wait();
            auto response = messagesRecv->pop_front();
            if (response.msg.id == MsgType::AdminConnectionEstablished)
                std::cout << "Login sucessful" << std::endl;
            else
                std::cout << "Username or Password incorrect" << std::endl;
        }

        void handleListConnectedUsers() {
            Net::Packet requestPacket;
            requestPacket.id = MsgType::ListConnectedUser;
            sendMessageToServer(requestPacket);
            if (messagesRecv->empty()) messagesRecv->wait();
            auto response = messagesRecv->pop_front();
            if (response.msg.id == MsgType::ConnectedUsers) {
                std::cout << "Connected Users:\n";
                for (size_t i = 0; i < response.msg.body.size(); i += sizeof(int)) {
                    int userId;
                    memcpy(&userId, &response.msg.body[i], sizeof(int));
                    std::cout << "User ID: " << userId << "\n";
                }
            }

        }

        void handleGetUserInfo(int userId) {
            Net::Packet msg;
            msg.id = MsgType::InfoUser;
            for (unsigned int i = 0; i < sizeof(userId); ++i) {
                msg.body.push_back((userId >> (i * 8)) & 0xFF);
            }
            sendMessageToServer(msg);
            if (messagesRecv->empty()) messagesRecv->wait();
            auto response = messagesRecv->pop_front();
            if (response.msg.id == MsgType::UserInfo) {
                std::cout << "User Info:\n";
                std::cout << MISCELLANEOUS::bytesToString(response.msg.body) << "\n";
            } else {
                std::cout << "Failed to retrieve user information." << std::endl;
            }
        }

        void handlebanUser(int userId) {
            Net::Packet msg;
            std::cout << userId;
            msg.id = MsgType::BanUser;
            std::cout << "before\n";
            msg << static_cast<uint8_t>(userId);
            std::cout << "after\n";
            sendMessageToServer(msg);
        }

        void handledebanUser(const std::string& ip) {
            Net::Packet msg;
            msg.id = MsgType::DeBan;
            for (auto c : ip) msg << c;
            sendMessageToServer(msg);
        }

        void handlelistUserBanned() {
            Net::Packet msg;
            msg.id = MsgType::ListUserBanned;
            sendMessageToServer(msg);
        }

        void handlegetInfoRoom(size_t roomId) {
            Net::Packet msg;
            msg.id = MsgType::InfoRoom;
            msg << static_cast<uint8_t>(roomId);
            sendMessageToServer(msg);
            if (messagesRecv->empty()) messagesRecv->wait();
            auto response = messagesRecv->pop_front();

            if (response.msg.id == MsgType::RoomInfo) {
                size_t maxPlayers = response.msg.body[0];
                size_t nbPlayerReady = response.msg.body[1];
                size_t state = response.msg.body[2];
                size_t playerCreator = response.msg.body[3];

                std::cout << "Room ID: " << roomId << std::endl;
                std::cout << "Max Players: " << maxPlayers << std::endl;
                std::cout << "Players Ready: " << nbPlayerReady << std::endl;
                std::cout << "Room State: " << state << std::endl;
                std::cout << "Player Creator: " << playerCreator << std::endl;
            }
        }

        void handleadminListRoom() {
            Net::Packet msg;
            msg.id = MsgType::AdminListRoom;
            sendMessageToServer(msg);

            if (messagesRecv->empty()) messagesRecv->wait();
            auto response = messagesRecv->pop_front();

            if (response.msg.id == MsgType::RoomList) {
                if (response.msg.body.empty()) {
                    std::cout << "No rooms available." << std::endl;
                } else {
                    std::cout << "Room List:" << std::endl;
                    for (auto &roomId : response.msg.body) {
                        std::cout << "Room ID: " << static_cast<size_t>(roomId) << std::endl;
                    }
                }
            } else {
                std::cerr << "Failed to get the room list." << std::endl;
            }
        }

        void getAllTotalPackets() {
            Net::Packet msg;
            msg.id = MsgType::GetAllTotalPackets;
            sendMessageToServer(msg);
            if (messagesRecv->empty()) messagesRecv->wait();
            auto response = messagesRecv->pop_front();

            if (response.msg.id == MsgType::TotalPackets) {
                unsigned long long totalPackets;
                totalPackets = *(reinterpret_cast<unsigned long long*>(response.msg.body.data()));

                std::cout << "Total Packets: " << totalPackets << std::endl;
            } else {
                std::cerr << "Failed to get the total number of packets." << std::endl;
            }
        }

        void setMaxRoom(int newMax) {
            Net::Packet msg;
            msg.id = MsgType::SETMAXROOM;
            msg << static_cast<uint8_t>(newMax);

            sendMessageToServer(msg);
        }

        void setCycle(int newMax) {
            Net::Packet msg;
            msg.id = MsgType::SETCYCLE;
            msg << static_cast<uint8_t>(newMax);

            sendMessageToServer(msg);
        }

         void handleAdminDeconnectionRequest() {
            Net::Packet msg;
            msg.id = MsgType::AdminDeconnectionRequest;
            sendMessageToServer(msg);
            if (messagesRecv->empty()) messagesRecv->wait();
        }

    private:
        boost::asio::io_context asioContext;
        std::thread threadContext;
        Network communication;
        std::shared_ptr<Thread::ThreadSafeQueue<ownedPacket>> messagesRecv;
        std::unordered_set<boost::asio::ip::udp::endpoint> adminEndpoints;
        size_t admin_port;
        size_t server_port;
        boost::asio::ip::udp::endpoint server;

        bool running = true;
    };

}


#endif /* !SERVER_HPP_ */
