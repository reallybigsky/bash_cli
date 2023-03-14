#ifndef BASH_CLI_ECHO_HPP
#define BASH_CLI_ECHO_HPP

#include "cmd.hpp"

#include <cstddef>
#include <numeric>

namespace commands {
/**
 * Implementation of echo command
 */
class Echo : public Cmd {
public:
    /**
     * Echo tok.args to output stream
     * How it is written in interpreter syntax:  <echo some text ...>
     * Absence of tok.args is valid, outputs an empty string
     * echo
     *
     *
     * @param params: token with command name in tok.name and command arguments in tok.args
     * @param env: current environment variables of the interpreter
     * @param input: input FILE stream (unused)
     * @param output: output FILE stream
     * @return 0 always
     */
    virtual int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE*) override {
        std::stringstream result;
        if (params.args.empty()) {
            result << std::endl;
        } else {
            result << std::accumulate(std::begin(params.args) + 1,
                                      std::end(params.args),
                                      params.args[0],
                                      [](std::string s0, std::string const& s1) { return s0 += " " + s1; })
            << std::endl;
        }

        FileUtils::writeToFile(result.str(), output);
        return 0;
    }
};

}

#endif //BASH_CLI_ECHO_HPP
