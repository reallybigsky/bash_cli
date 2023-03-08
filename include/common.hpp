#ifndef BASH_CLI_COMMON_HPP
#define BASH_CLI_COMMON_HPP

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <unordered_map>

//TODO: documentation

struct job {
    std::string name;
    std::vector<std::string> args;

    /**
     * Add a word to job: if name empty assigne this word to name,
     *                    else add word to args.
     *
     * @param `word` strings - some word: command name or argument.
     */
    void add_word(const std::string & word) {
        if (name.empty()) {
          name = word;
        } else {
          args.push_back(word);
        }
    }
};

struct EnvState {
//    EnvState(std::filesystem::path p, std::unordered_map<std::string, std::string> v, std::ostream& es)
//        : path(p)
//        , varEnv(v)
//        , serr(es) {}

    EnvState(std::filesystem::path p, std::ostream& es)
            : path(p)
            , varEnv()
            , serr(es) {}

    std::filesystem::path path;
    std::unordered_map<std::string, std::string> varEnv;
    std::ostream& serr;
};

#endif //BASH_CLI_COMMON_HPP
