/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** EnemyManager
*/

#ifndef ENEMYMANAGER_HPP_
    #define ENEMYMANAGER_HPP_
    #include "dependencies.hpp"
    #include <filesystem>
    #include <fstream>
    #include <sys/stat.h>
    #include "shooter.hpp"
    #include "property.hpp"
    #include "drawableType.hpp"
    #include "CollisionTrigger.hpp"
    #include "../../Engine/scene/scene.hpp"
    #include "../../Modules/miscellaneous.hpp"
    #include "../../Engine/Builtins/Components/boxCollider.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/Builtins/Components/transform.hpp"
    #include "../../Engine/Builtins/Components/orientation.hpp"
    #include "../../Engine/Builtins/Components/gravity.hpp"
    #include "../../Engine/Builtins/Components/updateTime.hpp"
    #include "../../Engine/Builtins/Components/velocity.hpp"
    #include "../../Engine/Builtins/Components/acceleration.hpp"
    #include <cstdlib>

    #if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #else
        #include <dirent.h>
        #include <sys/types.h>
    #endif

    #define BASIC_HEIGHT 34 
    #define BASIC_WIDTH 65
    #define MEDIUM_HEIGHT 33
    #define MEDIUM_WIDTH 33
    #define ADVANCED_HEIGHT 34
    #define ADVANCED_WIDTH 33
    #define BOSS_HEIGHT 208
    #define BOSS_WIDTH 171

using namespace MISCELLANEOUS;

class EnemyManager : public BJENGINE::ASystem {
    public:
        using chrono = std::chrono::time_point<std::chrono::system_clock>;
    public:
        EnemyManager(std::shared_ptr<BJENGINE::registry> reg, const int &c, std::string enemies_dir) : BJENGINE::ASystem(reg), map(enemies_dir), cycle(c) {}

        ~EnemyManager() {}

        std::vector<std::string> listFiles(const std::string& directory) {
        std::vector<std::string> files;
        #if defined(_WIN32) || defined(_WIN64)
            std::string search_path = directory + "/*.*";
            WIN32_FIND_DATA fd; 
            HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
            if(hFind != INVALID_HANDLE_VALUE) { 
                do { 
                    if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                        std::cout << fd.cFileName << std::endl;
                        files.push_back(fd.cFileName);
                    }
                } while(::FindNextFile(hFind, &fd)); 
                ::FindClose(hFind); 
            } else {
                std::cerr << "Erreur." << std::endl;
            }
        #else 
            DIR *dir;
            struct dirent *entry;

            if ((dir = opendir(directory.c_str())) != NULL) {
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_type != DT_DIR) {
                        files.push_back(entry->d_name);
                    }
                }
                closedir(dir);
            } else {
                std::vector<std::string> files;
                std::cerr << "Erreur." << std::endl;
            }
        #endif
        return files;
        }

        void start() override
        {
            std::vector<std::string> files = listFiles(map);
            for (auto tmp : files) {
                tmp = map + "/" + tmp;
                std::cout << tmp << std::endl;
                std::ifstream ifs(tmp);
                std::string s;
                std::getline(ifs, s);
                std::getline(ifs, s);
                std::cout << s << std::endl;
                enemyData.push_back(split(s, ' '));
                std::vector<chrono> ch;
                ch.push_back(std::chrono::system_clock::now());
                ch.push_back(std::chrono::system_clock::now());
                timesCounter.push_back(ch);
            }
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
        }

        void stop() override
        {
        }

        void spawnNewEnemy(int id)
        {
            auto newEnemy = _registry->spawn_entity();
            glm::vec2 pos(enemyData[id][5] == "RAND" ? (std::rand() % 1920) : std::stoi(enemyData[id][5]), (enemyData[id][6] == "RAND" ? (std::rand() % 1000) : std::stoi(enemyData[id][6])));
            glm::vec2 vc(std::stoi(enemyData[id][7]), std::stoi(enemyData[id][8]));
            ORIENTATION o;
            drawableType t;

            o = orientations[enemyData[id][10]];
            t = draws[enemyData[id][0]];
            shooter s(std::stoi(enemyData[id][2]), fires[enemyData[id][1]], std::stoi(enemyData[id][3]),  std::stoi(enemyData[id][4]));
            float h, w;
            if (t == drawableType::ENEMY_BASIC) {
                h = BASIC_HEIGHT;
                w = BASIC_WIDTH;
            } else if (t == drawableType::ENEMY_MEDIEUM) {
                h = MEDIUM_HEIGHT;
                w = MEDIUM_WIDTH;
            } else if (t == drawableType::ENEMY_ADVANCED) {
                h = ADVANCED_HEIGHT;
                w = ADVANCED_WIDTH;
            }
            else {
                h = BOSS_HEIGHT;
                w = BOSS_WIDTH;
            }
            boxCollider b(0, 0, h, w);
            b.tag = "enemy";
            property p(std::stoi(enemyData[id][14]), std::stoi(enemyData[id][13]));
            _registry->attach_components(newEnemy, transform(pos), velocity(vc), orientation(o), acceleration(), gravity(), drawable(t), updateTime(500), s, b, p);
        }

        void checkEnemyActivity(void)
        {
            for (int i = 0, size = enemyData.size(); i < size; i++) {
              //  std::cout << enemyData[i][10] << std::endl;
              //  std::cout << "[SERVER] checking enemy activity" << std::endl;
                if (cycle > std::stoi(enemyData[i][12])) {
                    enemyData.erase(enemyData.begin() + i);
                    break;
                } else if (std::stoi(enemyData[i][11]) <= cycle) {
                    now = std::chrono::system_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - timesCounter[i][0]).count();
                    if (duration >= std::stoi(enemyData[i][9])) {
                    //    std::cout << "[SERVER] new enemie created" << std::endl;
                        spawnNewEnemy(i);
                        timesCounter[i][0] = std::chrono::system_clock::now();
                    }
                }
            }
        }

        void update() override
        {
            checkEnemyActivity();
        }

    protected:
        std::string map;
        const int &cycle;
        chrono now;
        std::vector<std::vector<chrono>> timesCounter;
        std::vector<std::vector<std::string>> enemyData;

    protected:
        std::map<std::string, ORIENTATION> orientations = {{"LEFT",  ORIENTATION::Left},
                                                           {"RIGHT", ORIENTATION::Right},
                                                           {"UP",    ORIENTATION::Up},
                                                           {"DOWN",  ORIENTATION::Down}};

        std::map<std::string, drawableType> draws = {{"basic",    drawableType::ENEMY_BASIC},
                                                     {"medieum",  drawableType::ENEMY_MEDIEUM},
                                                     {"advanced", drawableType::ENEMY_ADVANCED},
                                                     {"BOSS",     drawableType::BOSS}};

        std::map<std::string, drawableType> fires = {{"fire1", drawableType::FIRE1},
                                                     {"fire2", drawableType::FIRE2},
                                                     {"fire3", drawableType::FIRE3},
                                                     {"fire4", drawableType::FIRE4}};
    private:
};

#endif /* !ENEMYMANAGER_HPP_ */
