#ifndef BASH_CLI_ASSIGNMENT_HPP
#define BASH_CLI_ASSIGNMENT_HPP

#include "commands_utils.h"
#include <regex>

namespace commands {

    class Assignment : public Cmd {
    public:
        virtual int run(const job &params, EnvState &env, std::istream &in, std::ostream &out) override {

            if(params.args.empty())
                throw std::invalid_argument("No files were transferred");

            std::stringstream result;
            if (params.args.size() > 1) {
                std::regex spec_symbols(R"('|"|\\|\$)");
                result << std::regex_replace(params.args[1], spec_symbols, "\\$&");
            } else {
                result << "";
            }

            env.varEnv[params.args[0]] = result.str();

            return 0;
        }

    };

}


#endif //BASH_CLI_ASSIGNMENT_HPP
