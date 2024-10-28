#include <iostream>
#include <string>
#include "ServerAdmin.hpp"

using namespace MISCELLANEOUS;

void help(void) {
    std::cout << "Usage: admin_cli  <server_address> <port>\n\n";
    std::cout << "Commands:\n";
    std::cout << "  stop                          - Arrête le serveur d'administration.\n";
    std::cout << "  connect <username> <password> - Connecte l'administrateur au serveur d'administration.\n";
    std::cout << "  listconnecteduser             - Liste tous les utilisateurs connectés.\n";
    std::cout << "  getuserinfo <userid>          - Obtient des informations sur l'utilisateur spécifié par son ID.\n";
    std::cout << "  banuser <userID>              - Bannit l'utilisateur spécifié par son ID.\n";
    std::cout << "  debanuser <userIP>            - Débanne l'utilisateur spécifié par son IP.\n";
    std::cout << "  listuserbanned                - Liste tous les utilisateurs bannis.\n";
    std::cout << "  getinforoom <roomID>          - Obtient des informations sur la salle spécifiée par son ID.\n";
    std::cout << "  listroom                      - Liste toutes les salles.\n";
    std::cout << "  setmaxroom <newMax>           - Définit le nombre maximal de salles.\n";
    std::cout << "                                 Le nombre doit être entre 0 et " << static_cast<int>(std::numeric_limits<uint8_t>::max()) << ".\n";
    std::cout << "\nAssurez-vous de vous connecter avant d'essayer d'utiliser les autres commandes.\n";
}


void help(std::string bin)
{
    std::cerr << "Help:" << std::endl;
    std::cerr << bin << ": <ip> <port>" << std::endl;
}


int main(int ac, char const *argv[]) 
{
    if (ac != 3) {
        help(argv[0]);
        return 1;
    }
    std::cout << "Starting Admin Server..." << std::endl;
    Net::AdminServer adminServer(5557);
    adminServer.connectToServer(argv[1], argv[2]);
    if (!adminServer.start()) {
        std::cerr << "Failed to start Admin Server." << std::endl; return 1;
    }
    std::string line;
    while (true) {
        std::cout << "admin-cli > ";
        std::getline(std::cin, line);
        std::vector<std::string> command = split(line, ' '); 
        if (command.size() == 0) continue;
	    else if (command[0] == "help") help();
        else if (command[0] == "connect") adminServer.loginToServer(command);
        else if (command[0] == "listconnecteduser") {
            adminServer.handleListConnectedUsers();
        }
        else if (command[0] == "getuserinfo" ) {
            if (command.size() < 2) {
                std::cout << "Usage: getuserinfo <userid>" << std::endl;
                continue;
            }
            int userId = std::stoi(command[1]);
            adminServer.handleGetUserInfo(userId);
        }
        else if (command[0] == "banuser") {
            if (command.size() < 2) {
                std::cout << "Usage: banuser <userID>\n";
            } else {
                int userId = std::stoi(command[1]);
                adminServer.handlebanUser(userId);
            }
        } else if (command[0] == "debanuser") {
            if (command.size() < 2) {
                std::cout << "Usage: debanuser <userIP>\n";
            } else {
                adminServer.handledebanUser(command[1]);
            }
        } else if (command[0] == "listuserbanned") {
             adminServer.handlelistUserBanned();
        }
        else if (command[0] == "getinforoom") {
            if (command.size() < 2) {
                std::cout << "Usage: getinforoom <roomID>\n";
            } else {
                size_t roomId = static_cast<size_t>(std::stoul(command[1]));
                adminServer.handlegetInfoRoom(roomId);
            }
        }
        else if (command[0] == "listroom") {
             adminServer.handleadminListRoom();
        }
        else if (command[0] == "allpaquets") {
             adminServer.getAllTotalPackets();
        } else if (command[0] == "stop") {
            std::cout << "Stop is touch" << std::endl;
            adminServer.handleAdminDeconnectionRequest();
            break;
        } else if (command[0] == "setmaxroom") {
            if (command.size() < 2) {
                std::cerr << "Usage: setmaxroom <newMax>" << std::endl;
            } else {
                int newMax;
                try {
                    newMax = std::stoi(command[1]);
                    if (newMax <= 0 || newMax > std::numeric_limits<uint8_t>::max()) {
                        std::cerr << "Invalid max room size. Must be between 0 and " << static_cast<int>(std::numeric_limits<uint8_t>::max()) << "." << std::endl;
                    } else {
                        adminServer.setMaxRoom(newMax);
                    }
                } catch (const std::invalid_argument& ia) {
                    std::cerr << "Invalid number: " << command[1] << std::endl;
                } catch (const std::out_of_range& oor) {
                    std::cerr << "Number out of range: " << command[1] << std::endl;
                }
            }
        }
        else if (command[0] == "setcycle") {
            if (command.size() < 2) {
                std::cerr << "Usage: setcycle <newMax>" << std::endl;
            } else {
                int newMax;
                try {
                    newMax = std::stoi(command[1]);
                    if (newMax <= 0 || newMax > std::numeric_limits<uint8_t>::max()) {
                        std::cerr << "Invalid max cycle size. Must be between 0 and " << static_cast<int>(std::numeric_limits<uint8_t>::max()) << "." << std::endl;
                    } else {
                        adminServer.setCycle(newMax);
                    }
                } catch (const std::invalid_argument& ia) {
                    std::cerr << "Invalid number: " << command[1] << std::endl;
                } catch (const std::out_of_range& oor) {
                    std::cerr << "Number out of range: " << command[1] << std::endl;
                }
            }
        }
        else std::cout << "Invalid Command" << std::endl;
    }
    adminServer.stop();
    std::cout << "Admin Server stopped !!!" << std::endl;
    return 0;
}