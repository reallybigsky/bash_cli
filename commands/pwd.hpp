#ifndef BASH_CLI_PWD_HPP
#define BASH_CLI_PWD_HPP

#include "commands_utils.hpp"
#include <regex>
#include <iostream>

namespace  commands {

    class Pwd: public Cmd {
    public:
        virtual int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output) override{
            std::stringstream result;
            result << (*env)["PATH"].to_string() << std::endl;
            writeToFile(result.str(), output);
            return 0;
        }
    };
}


#endif //BASH_CLI_PWD_HPP
