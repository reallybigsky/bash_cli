#ifndef BASH_CLI_ECHO_HPP
#define BASH_CLI_ECHO_HPP

#include <cstddef>
#include <numeric>

namespace commands {

    class Echo : public Cmd {
    public:
        virtual int run(const job &params, EnvState &env, boost::process::ipstream &in, boost::process::opstream &out) override {
            out << std::accumulate(std::begin(params.args) + 1, std::end(params.args), params.args[0],
                                   [](std::string s0, std::string const& s1) { return s0 += " " + s1; }) << std::endl;
            return 0;
        }
    };

}

#endif //BASH_CLI_ECHO_HPP
