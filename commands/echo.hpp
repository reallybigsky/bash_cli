#ifndef BASH_CLI_ECHO_HPP
#define BASH_CLI_ECHO_HPP

#include "basecmd.hpp"

class Echo : public Cmd {
    virtual int run(const job&, EnvState&, std::istream&, std::ostream&) override {
        return 0;
    }
};

#endif //BASH_CLI_ECHO_HPP
