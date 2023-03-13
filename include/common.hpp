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

/**
 * Represents command name with its arguments after syntax analysis
 */
struct token {
    std::string name; /// name of the command
    std::vector<std::string> args; /// command arguments

    /**
     * Add a word to token: if name is empty then assigns this word to token.name,
     *                    else add word to token.args.
     *
     * @param word: command name or argument.
     */
    void add_word(const std::string& word) {
        if (name.empty()) {
          name = word;
        } else {
          args.push_back(word);
        }
    }
};

#endif //BASH_CLI_COMMON_HPP
