#ifndef BASH_CLI_ECHO_HPP
#define BASH_CLI_ECHO_HPP

#include "cmd.hpp"
#include <cstddef>
#include <numeric>

namespace commands {

    class Echo : public Cmd {
    public:
        virtual int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output) override {
            std::stringstream result;
            result << std::accumulate(std::begin(params.args) + 1, std::end(params.args), params.args[0],
                                   [](std::string s0, std::string const& s1) { return s0 += " " + s1; }) << std::endl;

            writeToFile(result.str(), output);
            return 0;
        }
    };

}


#endif //BASH_CLI_ECHO_HPP
