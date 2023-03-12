#ifndef BASH_CLI_PWD_HPP
#define BASH_CLI_PWD_HPP

#include "cmd.hpp"
#include "file_utils.hpp"

#include <regex>
#include <iostream>


namespace  commands {

class Pwd: public Cmd {
public:
    virtual int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE* err) override {
        std::stringstream result;
        result << env->at("PWD").to_string() << std::endl;
        FileUtils::writeToFile(result.str(), output);
        return 0;
    }
};

} // namespace commands

#endif //BASH_CLI_PWD_HPP
