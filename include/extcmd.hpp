#ifndef BASH_CLI_EXTCMD_HPP
#define BASH_CLI_EXTCMD_HPP

#include "basecmd.hpp"


class ExtCmd : public BaseCmd {
public:
    virtual int run(const job&, EnvState&, std::istream&, std::ostream&) override {
        return 0;
    }
};

#endif //BASH_CLI_EXTCMD_HPP
