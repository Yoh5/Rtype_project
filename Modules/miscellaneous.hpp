#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <memory>


namespace MISCELLANEOUS {
    std::vector<std::string> split(std::string s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);  
    std::string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
    }

    bool str_contains(std::vector<std::string> list, std::string str) {
        return (std::find(list.begin(), list.end(), str) != list.end());
    }
    template <typename t>
    bool vector_contains(t e, std::vector<t> list) {
        return (std::find(list.begin(), list.end(), e) != list.end());
    }
    std::vector<int> strings_to_ints(std::string s)
    {
        std::vector<int> result;
        std::vector<std::string> ints = split(s, 32);
        for (auto elem : ints)
            result.push_back(std::stoi(elem));
        return result;
    }

    std::string delete_command(std::string s) {
        int index = 0;
        while (s[index] != ' ' && index < s.size()) index++;
        if (s[index] == ' ') index++;
        return (std::string(s.begin() + index, s.end()));
    }

    std::string bytesToString(const std::vector<uint8_t>& vec)
    {
        std::string res;
        for (auto c : vec) res.push_back((char)c);
        return res;
    }

    Net::Packet stringToBytes(const std::string s)
    {
        Net::Packet m;
        for (auto c : s) m << c;
        return m;
    }
}
