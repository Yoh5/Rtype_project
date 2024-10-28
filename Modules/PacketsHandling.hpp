/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** Serialize
*/

#ifndef HUFFMAN_COMPRESSION_HPP
    #define HUFFMAN_COMPRESSION_HPP
    #include <boost/asio.hpp>
    #include <vector>
    #include <cstdint>

////////////////////////////////////////////////////////////
// Define Packet Handling 
// Avoid Packet Losing
////////////////////////////////////////////////////////////

class PacketHandler {
private:
    boost::asio::ip::udp::socket& socket;
    std::vector<uint8_t> recvBuffer;
public:
    PacketHandler(boost::asio::ip::udp::socket& s) : socket(s) {}

////////////////////////////////////////////////////////////
// \brief Avoid Packet Sent Lost
// \param const std::vector<uint8_t>& packet  who is an boost::asio::ip::udp::endpoint&
// and const boost::asio::ip::udp::endpoint& endpoint
// \return fill boost::asio::ip::udp::socket& socket& socket
////////////////////////////////////////////////////////////

    void sendPacket(const std::vector<uint8_t>& packet, const boost::asio::ip::udp::endpoint& endpoint) {
        uint16_t size = static_cast<uint16_t>(packet.size());
        std::vector<uint8_t> dataToSend(2 + size);
        dataToSend[0] = size & 0xFF;
        dataToSend[1] = (size >> 8) & 0xFF;
        std::copy(packet.begin(), packet.end(), dataToSend.begin() + 2);
        socket.send_to(boost::asio::buffer(dataToSend), endpoint);
    }

////////////////////////////////////////////////////////////
// \brief Avoid Received Sent Lost
// \param senderEndpoint who is an boost::asio::ip::udp::endpoint&
// \return recvBuffer as std::vector<uint8_t> 
////////////////////////////////////////////////////////////

    std::vector<uint8_t> receivePacket(boost::asio::ip::udp::endpoint& senderEndpoint) {
        recvBuffer.resize(2);
        socket.receive_from(boost::asio::buffer(recvBuffer), senderEndpoint);
        uint16_t size = static_cast<uint16_t>(recvBuffer[0] | (recvBuffer[1] << 8));
        recvBuffer.resize(size);
        socket.receive_from(boost::asio::buffer(recvBuffer), senderEndpoint);
        return recvBuffer;
    }
};

#endif /* !HUFFMAN_COMPRESSION_HPP */
