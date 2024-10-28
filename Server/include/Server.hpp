/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** Server
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_
    #include "dependencies.hpp"
    #include "../../Modules/Data.hpp"
    #include "../../Modules/ThreadPool.hpp"
    #include "../../Modules/Network.hpp"

namespace Net {
    class ServerUDP {
        public:
            ServerUDP(size_t port) : server_port(port), communication(asioContext, port)
            {
                messagesRecv = std::make_shared<Thread::ThreadSafeQueue<ownedPacket>>();
                communication.setMessagesReceive(messagesRecv);
            }

            virtual ~ServerUDP()
            {
                stop();
            }

            bool start()
            {
                try {
                    communication.start();
                    threadContext = std::thread([this]() { asioContext.run(); });
                } catch (std::exception &e) {
                    std::cerr << "[SERVER ERROR]: " << e.what() << std::endl;
                    return false;
                }
                std::cout << "[SERVER] Started." << std::endl;
                return true;
            }

            void stop()
            {
                asioContext.stop();
                if (threadContext.joinable()) threadContext.join();
                std::cout << "[SERVER] Stopped." << std::endl;
            }

            size_t getPortServerRunning(void) const 
            {
                return server_port;
            }

            virtual bool isAClient(boost::asio::ip::udp::endpoint cli_end)
            {
                return false;
            }

            void sendMessageToClient(boost::asio::ip::udp::endpoint endpoint, const Packet &msg) 
            {
                communication.send(msg, endpoint);

            }

            virtual void sendMessageToAllClient(const Packet &msg) 
            {
            }

            void update(size_t nbMaxMessages = -1, bool wait = false)
            {
                if (wait) messagesRecv->wait();
                size_t n = 0;
				while (n < nbMaxMessages && !messagesRecv->empty())
				{
					auto msg = messagesRecv->pop_front();
					onMessage(msg.endpoint, msg.msg);
					n++;
				}
            }
        protected:

            virtual bool onClientConnect(boost::asio::ip::udp::endpoint  client, Packet msg) {
                return false;
            }
            
            virtual void onClientDisconnect(boost::asio::ip::udp::endpoint client)
            {
            }
            
            virtual void onMessage(boost::asio::ip::udp::endpoint client, Packet msg)
            {
            }

        protected:
            Packet messagesTemIn;
            boost::asio::ip::udp::endpoint tmp_cli_endpoint;
            boost::asio::io_context asioContext;
            std::thread threadContext;
            boost::asio::ip::udp::endpoint endpoint;
            size_t server_port;
            size_t clientIdCounter = 0;
//            Thread::ThreadSafeQueue<boost::asio::ip::udp::endpoint> client_endpoints;
            std::shared_ptr<Thread::ThreadSafeQueue<Net::ownedPacket>> messagesRecv;
            Network communication;
    };
}

#endif /* !SERVER_HPP_ */
