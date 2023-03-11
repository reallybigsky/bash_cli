#ifndef BASH_CLI_PWD_HPP
#define BASH_CLI_PWD_HPP

#include "cmd.hpp"
#include "commands_utils.hpp"

#include <regex>
#include <iostream>

namespace  commands {

class Pwd: public Cmd {
public:
    virtual int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE* err) override {
        std::stringstream result;
//        for (auto it = env->begin(); it != env->end(); ++it) {
//            result << it->get_name() << '\t' << it->to_string() << std::endl;
//        }
        result << env->at("PWD").to_string() << std::endl;
        FileUtils::writeToFile(result.str(), output);
        return 0;
    }
};

} // namespace commands

#endif //BASH_CLI_PWD_HPP
