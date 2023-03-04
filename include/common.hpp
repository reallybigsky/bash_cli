#ifndef BASH_CLI_COMMON_HPP
#define BASH_CLI_COMMON_HPP

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <unordered_map>

//TODO: documentation

struct msg {
    std::string name;
    std::vector<std::string> args;
};

struct EnvState {
    std::filesystem::path& path;
    std::unordered_map<std::string, std::string>& varEnv;
    std::istream& input;
    std::ostream& output;
};

#endif //BASH_CLI_COMMON_HPP
