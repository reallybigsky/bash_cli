#ifndef BASH_CLI_ECHO_HPP
#define BASH_CLI_ECHO_HPP

#include "cmd.hpp"

class Echo : public Cmd {
    virtual int run(const job& j, std::shared_ptr<Environment> e, FILE* input, FILE* output) override {
        for (const auto& arg : j.args) {
            writeToFile(arg + " ", output);
        }
        writeToFile("\n", output);
        return 0;
    }
};

#endif //BASH_CLI_ECHO_HPP
