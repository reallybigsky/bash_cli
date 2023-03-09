#ifndef BASH_CLI_ECHO_HPP
#define BASH_CLI_ECHO_HPP

#include "cmd.hpp"

class Echo : public Cmd {
    virtual int run(const job& j, std::shared_ptr<Environment> e, std::istream& is, std::ostream& os, std::ostream& es) override {
        for (const auto& arg : j.args) {
            os << arg << " ";
        }
        os << std::endl;
        return 0;
    }
};

#endif //BASH_CLI_ECHO_HPP
