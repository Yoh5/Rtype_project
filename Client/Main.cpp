#include "Client.hpp"

int main(int ac, char **av)
{
    RTYPE::Client cli(std::stoi(std::string(av[1])));
    cli.run();

    return 0;
}
