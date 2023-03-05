#ifndef BASH_CLI_COMMANDS_HPP
#define BASH_CLI_COMMANDS_HPP

#include "common.hpp"

#include <iostream>

//TODO: documentation

//TODO: ref of not ref EnvState

class BaseCmd {
public:
    virtual int run(const job&, EnvState&, std::istream&, std::ostream&) = 0;
};

class Cmd : public BaseCmd {
public:
    virtual int run(const job&, EnvState&, std::istream&, std::ostream&) = 0;
};

#endif //BASH_CLI_COMMANDS_HPP
