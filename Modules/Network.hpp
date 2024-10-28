/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** Network
*/

#ifndef Network_HPP_
    #define Network_HPP_
    #include "ThreadPool.hpp"
    #include "Data.hpp"
    #include "HuffmanBitset.hpp"
    #include <boost/asio/thread_pool.hpp>
    #define MAX_BODY_SIZE 80

namespace Net {
    class Network : public std::enable_shared_from_this<Network> {
        public:
            enum owner {server, client, admin};

            Network(boost::asio::io_context& ac, int port) : asioContext(ac), socket(ac, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
            {
            }

            ~Network() {}
            size_t getId(void) const
            {
                return id;
            }

            void setMessagesReceive(std::shared_ptr<Thread::ThreadSafeQueue<ownedPacket>> in)
            {
                messagesIn = in;
            }

            void start()
            {
                recvMessages();
            }

            void send(const Packet &msg, boost::asio::ip::udp::endpoint endpoint)
            {
                boost::asio::post(asioContext,
					[this, msg, endpoint]()
					{
                        bool bWritingMessage = !messagesOut.empty();
						messagesOut.push_back(std::make_pair(endpoint, msg));
						if (!bWritingMessage)
						{
                            writeMessages();
						}
					});
			}
            
            std::vector<uint8_t> compressData(std::vector<uint8_t> data)
            {
                Huffman huffman;
                std::string encoded;
                for (auto d : data) encoded.push_back(d);
                huffman.buildTree(encoded);
                return huffman.encode(encoded);
            }

            std::string decompressData(std::vector<uint8_t> compressed)
            {
                std::string decomp;
                return decomp;
            }

        private:

            void writeMessages(void)
            {
                int size_to_send = 1 + messagesOut.front().second.size();
                //std::vector<uint8_t> to_compress;
                //std::vector<uint8_t> compressed;
                buffer[0] = (uint8_t)messagesOut.front().second.id;
                //to_compress.push_back((uint8_t)messagesOut.front().second.id);
                //to_compress.insert(std::end(to_compress), std::begin( messagesOut.front().second.body), std::end( messagesOut.front().second.body));
                //compressed = compressData(to_compress);
                std::memcpy(buffer + 1, messagesOut.front().second.body.data(), size_to_send - 1);
             //   std::cout << "[Send] Buffer: " << (int)buffer[0] << std::endl;
                socket.async_send_to(boost::asio::buffer(buffer, size_to_send), messagesOut.front().first, 
                [this](std::error_code ec, std::size_t length) {
                    if (!ec) {
                        messagesOut.pop_front();
                        if (!messagesOut.empty())
                            writeMessages();
                    } else {
                        std::cerr << "Write Message Fail" << std::endl;
                        //add operation to delete this endpoint from list on endpoint
                    }
                });
            }

            void recvMessages(void)
            {
				socket.async_receive_from(boost::asio::buffer(buffer, MAX_BODY_SIZE), tmp_endpoint,
                    [this](std::error_code ec, std::size_t length)
                    {
                        if (!ec) {
                            if (isValidMessage(buffer, messagesTemIn, length)) {
                                //std::cout << buffer;
               //                 std::cout << "[RecV] Buffer: " << (int)buffer[0] << std::endl;
                                addToIncomingMessageQueue();
                            } else {
                                std::cout << "Invalid message" << std::endl;
                            }
                            recvMessages();
                        } else {
                            std::cout << "[" << id << "] Read Message Fail." << std::endl;
							//add operation to delete this endpoint
                        }
                    });
            }

            bool isValidMessage(char *buffer, Packet &messagesTemIn, size_t length)
            {
                if (length == 0)
                    return false;
                if ((unsigned int)buffer[0] > (unsigned int)MsgType::End) {
                    std::cerr << "Size too big" << std::endl;
                    return false;
                }
                messagesTemIn.id = (MsgType)buffer[0];
                messagesTemIn.body.resize(length - 1);
                std::memcpy(messagesTemIn.body.data(), buffer + 1, length - 1);
                return true;
            }

            void addToIncomingMessageQueue()
			{
				messagesIn->push_back({tmp_endpoint, messagesTemIn });
            }

        public:
            boost::asio::ip::udp::socket socket;
            boost::asio::io_context &asioContext;
            std::shared_ptr<Thread::ThreadSafeQueue<ownedPacket>> messagesIn;
            Packet messagesTemIn;
            char buffer[MAX_BODY_SIZE];
            boost::asio::ip::udp::endpoint tmp_endpoint;
            Thread::ThreadSafeQueue<std::pair<boost::asio::ip::udp::endpoint, Packet>> messagesOut;
            owner ownerType;
            size_t id;
    };
}

#endif /* !Network_HPP_ */
