#pragma once
#include <string>
#include <vector>

struct validationButton 
{
    validationButton(std::vector<int> req, std::string name) : _inputs(req), _name(name){} 
    std::vector<int> _inputs;
    std::string _name;
};