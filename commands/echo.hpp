#ifndef BASH_CLI_ECHO_HPP
#define BASH_CLI_ECHO_HPP

#include <cstddef>

namespace commands {

    class Echo : public Cmd {

        virtual int run(const job &params, EnvState &env, std::istream &in, std::ostream &out) override {
            for (size_t i = 0; i < params.args.size(); ++i) {
                out << params.args[i];
                if (i < params.args.size() - 1)
                    out << " ";
            }

            return 0;
        }
    };

}

#endif //BASH_CLI_ECHO_HPP
