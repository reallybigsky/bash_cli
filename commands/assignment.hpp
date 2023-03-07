#ifndef BASH_CLI_ASSIGNMENT_HPP
#define BASH_CLI_ASSIGNMENT_HPP

#include "commands_utils.h"
#include <regex>

namespace commands {

    class Assignment : public Cmd {
    private:
    public:
        virtual int run(const job &params, EnvState &env, std::istream &in, std::ostream &out) override {

            if(params.args.empty())
                throw std::invalid_argument("No files were transferred");


            std::stringstream result;
            std::regex spec_symbols(R"('|"|\\|\$)");
            for(size_t i = 1; i < params.args.size(); ++i){
                result << std::regex_replace(params.args[i], spec_symbols, "\\$&");

                if (i < params.args.size() - 1)
                    result << " ";
            }

            env.varEnv[params.args[0]] = result.str();

            return 0;
        }

    };

}


#endif //BASH_CLI_ASSIGNMENT_HPP
