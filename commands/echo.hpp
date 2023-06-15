#pragma once

#include "cmd.hpp"

#include <cstddef>
#include <numeric>

namespace Commands {

/**
 * Implementation of echo command
 */
class Echo : public Cmd {
public:
    /**
     * Echo params.args to output stream
     * How it is written in interpreter syntax:  <echo some text ...>
     * Absence of params.args is valid, outputs an empty string:
     * > echo
     * <empty line>
     *
     * @param params: CmdToken with command name in params.name and command arguments in params.args
     * @param env: current environment of the interpreter
     * @param input: input FileStream (unused)
     * @param output: output FileStream
     * @param err: error FileStream (unused)
     * @return 0 always
     */
    virtual int run(const CmdToken& params, std::shared_ptr<Environment>, FileStream&, FileStream& output, FileStream&) const override {
        if (!params.args.empty()) {
            output << params.args[0];
            for (size_t i = 1; i < params.args.size(); ++i)
                output << " " << params.args[i];
        }

        output << "\n";
        return 0;
    }
};

}
