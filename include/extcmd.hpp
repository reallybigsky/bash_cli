#ifndef BASH_CLI_EXTCMD_HPP
#define BASH_CLI_EXTCMD_HPP

#include "cmd.hpp"

class ExtCmd {
public:
    int run(const job&, std::shared_ptr<Environment>, FILE*, FILE*) {
        return 0;
    }
};

#endif //BASH_CLI_EXTCMD_HPP
