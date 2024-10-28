#pragma once
#include <map>
#include "../Modules/Data.hpp"

struct net
{
    net() : _server_ip(""), _server_port(-1){}
    std::string _server_ip;
    int _server_port;
};