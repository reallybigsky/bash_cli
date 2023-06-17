#pragma once

#include "common.hpp"
#include "file_utils.hpp"

/**
 * Base class of all internal commands of the interpreter
 *
 * Commands are called with run() function
 */
class Cmd {
public:
    /**
     * This function is called when internal command is executed
     *
     * @param params: CmdToken with command name in params.name and command arguments in params.args
     * @param env: current environment of the interpreter
     * @param input: input FileStream
     * @param output: output FileStream
     * @param err: error FileStream
     * @return 0 if there were no errors, 1 otherwise
     */
    virtual int run(const CmdToken& params, std::shared_ptr<Environment> env, FileStream& input, FileStream& output, FileStream& err) const = 0;

    virtual ~Cmd() = default;
};
