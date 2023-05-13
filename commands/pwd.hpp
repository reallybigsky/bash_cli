#pragma once

#include "cmd.hpp"
#include "file_utils.hpp"

#include <regex>
#include <iostream>


namespace  Commands {

/**
 * Implementation of echo command
 */
class Pwd: public Cmd {
public:
    /**
     * Print work directory to output stream
     * How it is written in interpreter syntax:  <pwd>
     *
     * All tok.args are ignored
     *
     * @param params: token with command name in tok.name and command arguments in tok.args
     * @param env: current environment variables of the interpreter
     * @param input: input FILE stream (unused)
     * @param output: output FILE stream
     * @param err: error FILE stream (unused)
     * @return 0 always
     */
    virtual int run(const token&, std::shared_ptr<Environment> env, FileStream&, FileStream& output, FileStream&) const override {
        output << env->current_path.string() << "\n";
        return 0;
    }
};

} // namespace commands
