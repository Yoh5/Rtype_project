/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** main
*/

#include "../include/RtypeServer.hpp"

void help(std::string bin)
{
    std::cerr << "help:" << std::endl;
    std::cout << bin << " <port> <enemy_map>" << std::endl;
}

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        help(argv[0]);
        return -1;
    }
    int port = std::stoi(argv[1]);
    RtypeNet::RtypeServer server(port, argv[2]);
    server.start();
    while (1) {
        server.update(-1, true);
    }
    return 0;
}
