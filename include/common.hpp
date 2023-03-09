#ifndef BASH_CLI_COMMON_HPP
#define BASH_CLI_COMMON_HPP

#include "ioservice.hpp"

#include <boost/process/system.hpp>
#include <boost/process/environment.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <unordered_map>

//TODO: documentation

using Environment = boost::process::basic_native_environment<char>;
using pstream = boost::process::pstream;

enum CmdPos {
    inner  = 0,
    first  = 1,
    last   = 2,
    single = 3
};


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
    std::shared_ptr<Environment> vars;
    CmdPos cmdPos;
    std::shared_ptr<IOservice> ios;
    std::shared_ptr<pstream> ipsCurr;
    std::shared_ptr<pstream> opsCurr;
};

#endif //BASH_CLI_COMMON_HPP
