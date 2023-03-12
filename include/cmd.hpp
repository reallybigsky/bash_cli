#ifndef BASH_CLI_CMD_HPP
#define BASH_CLI_CMD_HPP

#include "common.hpp"


class Cmd {
public:
    virtual int run(const token&, std::shared_ptr<Environment>, FILE*, FILE*, FILE*) = 0;

    virtual ~Cmd() = default;
};

#endif //BASH_CLI_CMD_HPP
