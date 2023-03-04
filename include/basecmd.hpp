#ifndef BASH_CLI_COMMANDS_HPP
#define BASH_CLI_COMMANDS_HPP

#include "common.hpp"

#include <iostream>

//TODO: documentation

//TODO: ref of not ref EnvState

class BaseCmd {
public:
    virtual int run(const msg&, EnvState) = 0;
};

class PureCmd : public BaseCmd {
public:
    virtual int run(const msg&, EnvState) = 0;
};

class MutCmd : public BaseCmd {
public:
    virtual int run(const msg&, EnvState) = 0;
};

#endif //BASH_CLI_COMMANDS_HPP
