#ifndef BASH_CLI_PWD_HPP
#define BASH_CLI_PWD_HPP

#include "commands_utils.h"
#include <regex>
#include <iostream>

namespace  commands {

    class Pwd: public Cmd {
    public:
        virtual int run(const job &params, EnvState &env, bp::ipstream &in, bp::opstream &out) override {
            out << env.varEnv["PATH"].to_string() << std::endl;
            return 0;
        }
    };
}


#endif //BASH_CLI_PWD_HPP
