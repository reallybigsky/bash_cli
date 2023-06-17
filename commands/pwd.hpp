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
     * Print working directory to output stream
     * How it is written in interpreter syntax: <pwd>
     *
     * All params.args are ignored
     *
     * @param params: CmdToken with command name in params.name and command arguments in params.args
     * @param env: current environment of the interpreter
     * @param input: input FileStream (unused)
     * @param output: output FileStream
     * @param err: error FileStream (unused)
     * @return 0 always
     */
    virtual int run(const CmdToken&, std::shared_ptr<Environment> env, FileStream&, FileStream& output, FileStream&) const override {
        output << env->current_path.string() << "\n";
        return 0;
    }

};

} // namespace commands
