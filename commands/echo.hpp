#ifndef BASH_CLI_ECHO_HPP
#define BASH_CLI_ECHO_HPP

#include "cmd.hpp"

#include <cstddef>
#include <numeric>

namespace commands {

class Echo : public Cmd {
public:
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
