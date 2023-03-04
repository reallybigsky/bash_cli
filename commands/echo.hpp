#ifndef BASH_CLI_ECHO_HPP
#define BASH_CLI_ECHO_HPP

#include "basecmd.hpp"

class Echo : public PureCmd {
    virtual int run(const msg&, EnvState) override {
        return 0;
    }
};

#endif //BASH_CLI_ECHO_HPP
