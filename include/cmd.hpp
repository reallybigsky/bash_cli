#pragma once

#include "common.hpp"

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
     * @param tok: token with command name in tok.name and command arguments in tok.args
     * @param env: current environment variables of the interpreter
     * @param input: input FILE stream (unused)
     * @param output: output FILE stream (unused)
     * @param err: error FILE stream
     * @return 0 if there were no errors, 1 otherwise
     */
    virtual int run(const token& tok, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE* err) = 0;

    virtual ~Cmd() = default;
};
