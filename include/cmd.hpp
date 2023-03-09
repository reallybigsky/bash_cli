#ifndef BASH_CLI_CMD_HPP
#define BASH_CLI_CMD_HPP

#include "common.hpp"

#include <boost/process/system.hpp>

//TODO: documentation

class Cmd {
public:
    virtual int run(const job&, std::shared_ptr<Environment>, std::istream&, std::ostream&, std::ostream&) = 0;

    virtual ~Cmd() {}
};

#endif //BASH_CLI_CMD_HPP
