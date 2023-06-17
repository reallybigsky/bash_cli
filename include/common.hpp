#pragma once

#include <boost/process/environment.hpp>

#include <string>
#include <vector>
#include <cstdio>
#include <optional>
#include <filesystem>

/**
 * Interpreter environment
 */
struct Environment {
    Environment() : current_path(std::filesystem::current_path()) {}

    boost::process::basic_native_environment<char> vars; /// dictionary of variables with their values
    std::filesystem::path current_path;                  /// current working directory
};

/**
 * Command name with its arguments after syntax analysis
 */
struct CmdToken {
    std::string name;              /// name of the command
    std::vector<std::string> args; /// command arguments

    /**
     * Add a word to CmdToken: if name is empty then assigns this word to CmdToken.name,
     *                         else add word to CmdToken.args.
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

using PipeLine = std::vector<CmdToken>;
