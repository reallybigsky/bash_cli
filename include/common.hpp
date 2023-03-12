#ifndef BASH_CLI_COMMON_HPP
#define BASH_CLI_COMMON_HPP

#include <boost/process/environment.hpp>

#include <string>
#include <vector>
#include <cstdio>
#include <optional>

struct token;
using Environment = boost::process::basic_native_environment<char>;
using PipeLine = std::vector<token>;


struct token {
    std::string name;
    std::vector<std::string> args;

    /**
     * Add a word to token: if name empty assigne this word to name,
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

#endif //BASH_CLI_COMMON_HPP
