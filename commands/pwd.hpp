#ifndef BASH_CLI_PWD_HPP
#define BASH_CLI_PWD_HPP

#include "commands_utils.h"
#include <regex>
#include <iostream>

namespace  commands {

    class Pwd: public Cmd {
    public:
        virtual int run(const job &params, EnvState &env, std::istream &in, std::ostream &out) override {
            out << env.path.string();
            return 0;
        }
    };
}


#endif //BASH_CLI_PWD_HPP
