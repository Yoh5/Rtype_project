#pragma once
#include <optional>
#include <any>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <algorithm>
#include <iostream>
#include <map>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/write.hpp>
#include "../Modules/Data.hpp"
#include "../Modules/ThreadPool.hpp"
#include "../Modules/Network.hpp"


class NetworkModule
{
public:
    NetworkModule(size_t port) : port(port), communication(asioContext, port)
    {
        messagesRecv = std::make_shared<Thread::ThreadSafeQueue<Net::ownedPacket>>();
        communication.setMessagesReceive(messagesRecv);
        try
        {
            communication.start();
            threadContext = std::thread([this]()
                                        { asioContext.run(); });
        }
        catch (std::exception &e)
        {
            std::cerr << "[CLIENT ERROR]: " << e.what() << std::endl;
        }
        std::cout << "[CLIENT] Started." << std::endl;
    }


    ~NetworkModule()
    {
        asioContext.stop();
        if (threadContext.joinable())
            threadContext.join();
        std::cout << "[CLIENT] Stopped." << std::endl;
    }

    void send(const Net::Packet &packet, std::string ip, int port)
    {
        if (ip.empty() || port < 0)
        return;
        communication.send(packet, boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(ip), port));
    }

    void send(const Net::Packet &packet, boost::asio::ip::udp::endpoint endp)
    {
        communication.send(packet, endp);
    }

    Net::Packet fetch(bool wait)
    {
        if (wait) messagesRecv->wait();

        if (!messagesRecv->empty())
            return messagesRecv->pop_front().msg;
        Net::Packet empty;
        empty.id = MsgType::End;
        return empty;
    }

private:
    boost::asio::io_context asioContext;
    std::thread threadContext;
    size_t port;
    std::shared_ptr<Thread::ThreadSafeQueue<Net::ownedPacket>> messagesRecv;
    Net::Network communication;
};