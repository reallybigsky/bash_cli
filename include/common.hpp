#pragma once

#include <boost/process/environment.hpp>

#include <string>
#include <vector>
#include <cstdio>
#include <optional>
#include <filesystem>

/**
 * Represents interpretator environment
 */
struct Environment {
    Environment() : vars(), current_path(std::filesystem::current_path()) {}

    boost::process::basic_native_environment<char> vars;
    std::filesystem::path current_path;
};

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
    template <typename T>
    requires (std::convertible_to<T, std::string>)
    void add_word(T&& word) {
        if (name.empty()) {
          name = word;
        } else {
          args.push_back(word);
        }
    }
};

using PipeLine = std::vector<token>;
