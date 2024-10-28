#pragma once
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <string>
#include <SFML/Audio.hpp>
#include "../Error/error.hpp"

class AudioManager
{
    public:

    AudioManager(std::unordered_map<std::type_index, std::string> sounds, std::unordered_map<std::type_index, std::string> musics)
    {
        for (auto [type, file] : sounds)
        {
            if (!_sounds_buffers[type].loadFromFile(file))
            throw BJENGINE::Error("AudioManager constructor", "18", "Failed to load sound buffer");
            _sounds[type].setBuffer(_sounds_buffers[type]);
        }

        for (auto [type, file] : musics)
        {
            _musics[type].openFromFile(file);   
        }
    }

    template<typename TYPE>
    void playSound()
    {
        lookUp<TYPE>();

        _sounds[typeid(TYPE)].play();
    }
    
    template<typename TYPE>
    void playMusic(bool loop=false)
    {
        lookUp<TYPE>();
        _musics[typeid(TYPE)].setLoop(loop);
        _musics[typeid(TYPE)].play();
    }

    template<typename TYPE>
    void stopMusic()
    {
        lookUp<TYPE>();
        _musics[typeid(TYPE)].stop();
    }

    private:
    template<typename TYPE>
    void lookUp()
    {
        // if (!_sounds.contains(typeid(TYPE)))
        //     throw BJENGINE::Error("lookup", "35", "Unknown Sound/Music type");
        // if (!_musics.contains(typeid(TYPE)))
        //     throw BJENGINE::Error("lookup", "35", "Unknown Sound/Music type");
    }

    std::unordered_map<std::type_index, sf::Sound> _sounds;
    std::unordered_map<std::type_index, sf::SoundBuffer> _sounds_buffers;
    std::unordered_map<std::type_index, sf::Music> _musics;
};