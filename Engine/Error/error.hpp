#pragma once
#include <exception>
#include <string>

namespace BJENGINE {
    class Error : public std::exception{
        public:
        Error(std::string func_name, std::string line, std::string msg)
        {
            _error_msg = "[BJENGINE ERROR]: at function " + func_name + " line : "  + line + " : " + msg + "\n";
        }

        const char *what(void) const noexcept override
        {
            return _error_msg.c_str();
        }

        private:
        std::string _error_msg;
    };
}